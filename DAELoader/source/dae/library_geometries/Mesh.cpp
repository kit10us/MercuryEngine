// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dae/library_geometries/Mesh.h>
#include <dae/Document.h>
#include <unify/V3.h>
#include <unify/TexCoords.h>

using namespace dxi;
using namespace dae;

typedef std::vector< float > VFloat;
typedef std::vector< int > VInt;

/// Integrates, into an existing vertex buffer, a component "dest", from "source", choriographed via "indices", offset by "indexOffset", every other "indexStride", until "count" is achieved.
void Integrate( dxi::VertexLock & lock, FVF::TYPE type, const VFloat & source, const VInt & indices, int indexOffset, int indexStride, size_t count )
{
	for( size_t i = 0; i < count; ++i )
	{
		size_t ii = indexOffset + i * indexStride;
		size_t p = indices[ ii ];
		switch( type )
		{
		case FVF::XYZ:
		case FVF::Normal:
			{
				unify::V3< float > v( source[ p * 3 + 0 ], source[ p * 3 + 1 ], source[ p * 3 + 2 ] );
				lock.SetItemMember( i, &v, type );
			}
			break;
		case FVF::Tex1:
		case FVF::Tex2:
		case FVF::Tex3:
		case FVF::Tex4:
			{
				unify::TexCoords uv( source[ p * 2 + 0 ], source[ p * 2 + 1 ] );
				lock.SetItemMember( i, &uv, type );
			}
			break;
		}
	}
}


Mesh::Mesh( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "source" ) )
		{
			std::shared_ptr< Source > source( new Source( document, childNode ) );
			m_source.push_back( source );
			m_source_map[ source->GetID() ] = m_source.size() - 1;
		}
		else if ( childNode->IsTagName( "vertices" ) )
		{
			m_vertices.reset( new Vertices( document, childNode ) );
		}
		else if ( childNode->IsTagName( "polylist" ) )
		{
			// polylist.count	- The number of faces (polylist.vcount size).
			// polylist.input*	- Dictates the layout of the vertex.
			// polylist.vcount	- Array of vertex counts, each represents the number of vertices in a face.
			// polylist.p		- Sets (size by number of inputs) alternating index into inputs to describe the actual vertices used.
			// polylist.p.size / polylist.input.size = number of vertices = sum of polylist.vcount
			// polylis.count = number of polylist.vcount = number of faces
			std::shared_ptr< Polylist > polylist( new Polylist( document, childNode ) );
			m_polylist.push_back( polylist );
		}
		else if ( childNode->IsTagName( "triangles" ) )
		{
			// NOTE: Using polylist as it reduces duplicate code.
			// An * represents a value that is not native to the triangle, it's 
			// triangles.count	- Number of triangle primities.
			// triangles.input* - Dicates the layout of the vertex.
			// *polylist.vcount = 3 - This is constant
			// triangles.p
			std::shared_ptr< Polylist > polylist( new Polylist( document, childNode ) );
			m_polylist.push_back( polylist );
		}
	}
}

const std::vector< std::shared_ptr< Source > > & Mesh::GetSource() const
{
	return m_source;
}

std::shared_ptr< Source > Mesh::GetSource( const std::string & name ) const
{
	std::string realName;
	
	// Remove prepended "#" from name if it exists...
	if ( name.at( 0 ) == '#' )
	{
		realName = name.substr( 1 );
	}
	else
	{
		realName = name;
	}

	// TODO: HACK: If we want the source, and the source is the vertices, then we really want to get it's position source. This will break when we get more complicate models,
	// however, it will also be far easier to develope the alternative then (we will have a real world example).
	if ( realName == m_vertices->GetID() )
	{
		realName = m_vertices->GetInput()[ 0 ]->GetSource();
		if ( realName.at( 0 ) == '#' )
		{
			realName = realName.substr( 1 );
		}
	}

	std::map< std::string, size_t >::const_iterator itr = m_source_map.find( realName );
	if ( itr == m_source_map.end() )
	{
		return std::shared_ptr< Source >();
	}

	return GetSource().at( itr->second );
}

void Mesh::Build( PrimitiveList & accumulatedPL, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const
{
	for ( std::vector< std::shared_ptr< Polylist > >::const_iterator itr = m_polylist.begin(), end = m_polylist.end(); itr != end; ++itr )
	{
		const Polylist & polylist = **itr;
		PrimitiveList pl;
		unify::Color diffuse = unify::Color::ColorWhite();
		const NewParam * sampler2DParam = 0;
		const NewParam * surfaceParam = 0;
		
		const std::string target = technique.GetInstanceMaterial( polylist.GetMaterial() ).GetTarget();
		const Material * material = GetDocument().GetLibraryMaterials().Find( target );
		std::string effectURL = material->GetInstanceEffect().GetURL();
		const Effect * effect = GetDocument().GetLibraryEffects().Find( effectURL );

		const Shading & shading = effect->GetProfileCOMMON()->GetTechnique().GetShading();
		dxi::Effect::shared_ptr primitiveEffectBase = GetDocument().GetEffect( shading );
		dxi::Effect::shared_ptr myEffect( new dxi::Effect() );
		*myEffect = *primitiveEffectBase; // Copy.
		myEffect->ClearTextures(); // Clear textures, as we don't want the default texture that might have come with the effect.

		if ( shading.GetDiffuse().GetType() == Shading::Property::ColorType )
		{
			diffuse = unify::Color( shading.GetDiffuse().GetColor() );
			myEffect->SetTexture( 0, Texture::shared_ptr() ); // Unset texture.
		}
		else // Is texture...
		{
			sampler2DParam = effect->GetProfileCOMMON()->FindNewParam( shading.GetDiffuse().GetTexture() );
			surfaceParam = effect->GetProfileCOMMON()->FindNewParam( sampler2DParam->GetSampler2D().source );
			const dae::Image * image = GetDocument().GetLibraryImages().Find( surfaceParam->GetSurface().init_from );
			dxi::Texture::shared_ptr texture( image->GetTexture() );
			myEffect->SetTexture( 0, texture );
		}

		// Create our primitive list...
		size_t numberOfVertices = polylist.GetP().size() / polylist.GetInput().size();
		BufferSet & set = pl.AddBufferSet();
		set.GetVertexBuffer().Create( numberOfVertices, myEffect->GetVertexShader()->GetVertexDeclaration(), BufferUsage::Staging );

		VertexWriter::shared_ptr vw = myEffect->GetVertexShader()->GetVertexDeclaration().GetVertexWriter();

		VertexLock lock;
		set.GetVertexBuffer().Lock( lock );

		// Iterate through the vertices...
		size_t vertexIndex = 0;
		for ( size_t pHead = 0; pHead < polylist.GetP().size(); pHead += polylist.GetInput().size(), ++vertexIndex )
		{
			for( size_t inputIndex = 0; inputIndex < polylist.GetInput().size(); ++inputIndex )
			{
				size_t indexIntoP = pHead + inputIndex; // this is the actual index into the P array, recalling that p = { { vertex0.input0, vertex0.input1}, {vertex1.input0, vertex1.input1 }, etc... }
				size_t indexOfAttribute = polylist.GetP()[ indexIntoP ];

				const dae::Input_Shared & input = *polylist.GetInput()[ inputIndex ];
				const dae::Source & source = *GetSource( input.GetSource() );
				const std::string metaType = input.GetSemantic(); // Type of data, meta as it's an assumption.
				const std::vector< float > & floats = source.GetFloatArray().GetArrayContents();
				size_t offsetOfFloats = indexOfAttribute * input.GetStride();

				if ( unify::StringIs( metaType, "POSITION" ) || unify::StringIs( metaType, "VERTEX" ) )
				{
					unify::V3< float > val( floats[ offsetOfFloats + 0 ] * -1.0f, floats[ offsetOfFloats + 1 ], floats[ offsetOfFloats + 2 ] );
					matrix.TransformCoord( val );
					vw->SetPosition( lock, vertexIndex, val );
					vw->SetDiffuse( lock, vertexIndex, diffuse );
				}
				else if ( unify::StringIs( metaType, "NORMAL" ) )
				{
					unify::V3< float > val( floats[ offsetOfFloats + 0 ], floats[ offsetOfFloats + 1 ], floats[ offsetOfFloats + 2 ] );
					vw->SetNormal( lock, vertexIndex, val );
				}
				else if ( unify::StringIs( metaType, "TEXCOORD" ) )
				{
					vw->SetTexCoordsN( lock, vertexIndex, 0, unify::TexCoords( floats[ offsetOfFloats + 0 ], floats[ offsetOfFloats + 1 ] * -1.0f ) );
				}
			}
		}

		struct VT
		{
			unify::V3< float > p;
			unsigned int d;
		};

		VT * vt = (VT*)lock.GetData();
		set.GetVertexBuffer().Unlock();


		size_t numberOfIndices = 0;
		for( size_t vci = 0; vci < polylist.GetVCount().size(); ++vci )
		{
			size_t vc = polylist.GetVCount()[ vci ];
			numberOfIndices += ( vc - 2 ) * 3;
		}

		set.GetIndexBuffer().Create( numberOfIndices, IndexFormat::Index32, BufferUsage::Staging );
		IndexLock iLock;
		set.GetIndexBuffer().Lock( iLock );

		size_t vertexHead = 0; // Tracks the first vertex, increases by vc each iteration.
		size_t index = 0;
		for( size_t vci = 0; vci < polylist.GetVCount().size(); ++vci )
		{
			size_t vc = polylist.GetVCount()[ vci ];
			for( size_t indexOffset = 0; indexOffset < vc - 2; ++indexOffset )
			{
				iLock.SetIndex( index++, vertexHead + indexOffset );
				iLock.SetIndex( index++, vertexHead + indexOffset + 1 );
				iLock.SetIndex( index++, vertexHead + vc - 1 );
			}
			vertexHead += vc;
		}

		set.GetIndexBuffer().Unlock();

		set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, numberOfVertices, numberOfIndices / 3, myEffect, true ) );

		accumulatedPL.Append( pl );
	}
}