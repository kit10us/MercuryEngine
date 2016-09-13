// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/GeometryFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/XMLConvert.h>
#include <qxml/Document.h>

using namespace dxi;

void LoadMesh_1_2( const qxml::Element & geometryElement, dxi::Mesh * mesh );

Geometry * GeometryXMLFactory::Produce( const qxml::Element & geometryElement )
{
	if ( ! geometryElement.HasAttributes( "version" ) )
	{
		return 0;
	}

	Mesh * mesh = new Mesh;

	std::string version{ geometryElement.GetAttribute< std::string >( "version" ) };
	if( version == "1.2" )
	{
		LoadMesh_1_2( geometryElement, mesh );
	}
	else
	{
		throw exception::FailedToCreate( "Geometry XML version " + version + " not supported!" );
	}

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );

	return mesh;
}


void LoadMesh_1_2( const qxml::Element & geometryElement, dxi::Mesh * mesh )
{
	core::Game & game = *dxi::core::Game::GetInstance();

	// Managers to store sub-resources.
	auto textureManager = game.GetManager< Texture >();
	auto pixelShaderManager = game.GetManager< PixelShader >();
	auto vertexShaderManager = game.GetManager< VertexShader >();
	auto effectManager = game.GetManager< Effect >();

	for( const auto child : geometryElement.Children() )
	{
		// Added effects.
		if( child.IsTagName( "effectlist" ) )
		{
			for( const auto effect: child.Children() )
			{
				unify::Path path;
				path.Combine( geometryElement.GetDocument()->GetPath().DirectoryOnly(), effect.GetAttribute< std::string >( "source" ) );
				effectManager->Add( effect.GetAttribute< std::string >( "name" ), path );
			}
		}

		if( child.IsTagName( "bufferset" ) )
		{
			BufferSet & bufferset = mesh->GetPrimitiveList().AddBufferSet();

			for( auto buffersetChild : child.Children() )
			{
				if( buffersetChild.IsTagName( "vertexlist" ) )
				{	
					std::string effectName = buffersetChild.GetAttribute< std::string >( "effect" );
					auto effect = effectManager->Find( effectName );
					VertexDeclaration vd = effect->GetVertexShader()->GetVertexDeclaration();
					unsigned int vertexCount = buffersetChild.GetAttribute< unsigned int >( "count" );
					VertexBuffer & vb = bufferset.GetVertexBuffer();

					vb.Create( vertexCount, vd ); // TODO: Additional flags and parameter supports.

					unify::DataLock lock;
					vb.Lock( lock );

					unsigned short stream = 0;

					D3DVERTEXELEMENT9 positionE = {};
					positionE.Stream = stream;
					positionE.Type = D3DDECLTYPE_FLOAT3;
					positionE.Usage = D3DDECLUSAGE_POSITION;
					positionE.UsageIndex = 0;

					D3DVERTEXELEMENT9 normalE = {};
					normalE.Stream = stream;
					normalE.Type = D3DDECLTYPE_FLOAT3;
					normalE.Usage = D3DDECLUSAGE_NORMAL;
					normalE.UsageIndex = 0;

					D3DVERTEXELEMENT9 diffuseE = {};
					diffuseE.Stream = stream;
					diffuseE.Type = D3DDECLTYPE_D3DCOLOR;
					diffuseE.Usage = D3DDECLUSAGE_COLOR;
					diffuseE.UsageIndex = 0;

					D3DVERTEXELEMENT9 specularE = {};
					specularE.Stream = stream;
					specularE.Type = D3DDECLTYPE_D3DCOLOR;
					specularE.Usage = D3DDECLUSAGE_COLOR;
					specularE.UsageIndex = 1;

					D3DVERTEXELEMENT9 texE = {};
					texE.Stream = stream;
					texE.Type = D3DDECLTYPE_FLOAT2;
					texE.Usage = D3DDECLUSAGE_TEXCOORD;
					texE.UsageIndex = 0;

					D3DVERTEXELEMENT9 texE2 = {};
					texE2.Stream = stream;
					texE2.Type = D3DDECLTYPE_FLOAT2;
					texE2.Usage = D3DDECLUSAGE_TEXCOORD;
					texE2.UsageIndex = 1;

					for( auto vertex : buffersetChild.Children( "vertex" ) )
					{  		
						size_t index = vertex.GetAttribute< size_t >( "index" );
	
						// XYZ
						unify::V3< float > xyz;
						if( XMLConvert( &vertex, xyz ) )
						{
							vd.WriteVertex( lock, index, positionE, xyz );
						}
	
						// NORMAL
						unify::V3< float > normal;
						if( vertex.HasAttributes( "nx" ) )
						{
							normal.x = vertex.GetAttribute< float >( "nx" );
						}
						if( vertex.HasAttributes( "ny" ) )
						{
							normal.y = vertex.GetAttribute< float >( "ny" );
						}
						if( vertex.HasAttributes( "nz" ) )
						{
							normal.z = vertex.GetAttribute< float >( "nz" );
						}
						if( vertex.HasAttributes( "normal" ) )
						{
							normal = unify::V3< float >( vertex.GetAttribute< std::string >( "normal" ) );
						}
						if( vertex.HasAttributes( "nx" ) || vertex.HasAttributes( "ny" ) || vertex.HasAttributes( "nz" ) || vertex.HasAttributes( "normal" ) )
						{
							vd.WriteVertex( lock, index, normalE, normal );
						}
	
						// TEXTURE 1
						unify::TexCoords uv;
						if( vertex.HasAttributes( "tu" ) )
						{
							uv.u = vertex.GetAttribute< float >( "tu" );
						}
						if( vertex.HasAttributes( "tv" ) )
						{
							uv.v = vertex.GetAttribute< float >( "tv" );
						}
						if( vertex.HasAttributes( "uv" ) )
						{
							uv = unify::TexCoords( vertex.GetAttribute< std::string >( "uv" ) );
						}
						if( vertex.HasAttributes( "tu" ) || vertex.HasAttributes( "tv" ) || vertex.HasAttributes( "uv" ) )
						{
							vd.WriteVertex( lock, index, texE, uv );
						}
	
						// TEXTURE 2
						unify::TexCoords uv2;
						if( vertex.HasAttributes( "tu2" ) )
						{
							uv2.u = vertex.GetAttribute< float >( "tu2" );
						}
						if( vertex.HasAttributes( "tv2" ) )
						{
							uv2.v = vertex.GetAttribute< float >( "tv2" );
						}
						if( vertex.HasAttributes( "uv2" ) )
						{
							uv2 = unify::TexCoords( vertex.GetAttribute< std::string >( "uv" ) );
						}
						if( vertex.HasAttributes( "tu2" ) || vertex.HasAttributes( "tv2" ) || vertex.HasAttributes( "uv2" ) )
						{
							vd.WriteVertex( lock, index, texE2, uv2 );
						}
	
						unify::ColorUnit diffuseUnit;
						if( XMLConvert( &vertex, diffuseUnit ) )
						{
							unify::Color diffuse( diffuseUnit );
							vd.WriteVertex( lock, index, diffuseE, diffuse );
						}
					}
				}	 
				else if( buffersetChild.IsTagName( "indexlist" ) )
				{
					unsigned int numIndices = unify::ListPartCount( buffersetChild.GetText(), {','} );

					IndexBuffer & ib = bufferset.GetIndexBuffer();
					ib.Create( numIndices, IndexFormat::Index16 );
					IndexLock indexLock;
					ib.Lock( indexLock );
					Index16* pIndex = (Index16 *)indexLock.GetData();

					// Load indices...
					for( unsigned int u = 0; u < numIndices; u++ )
					{
						pIndex[u] = (Index16)unify::Cast< int >( unify::ListPart( buffersetChild.GetText(), {','}, u ) );
					}
					ib.Unlock();
				}	 	
				else if( buffersetChild.IsTagName( "methodlist" ) )
				{
					for( auto methodElement : buffersetChild.Children( "addmethod" ) )
					{
						// Determine Primitive Type...
						PrimitiveType::TYPE pt;
						pt = PrimitiveType::StringToPrimitiveType( methodElement.GetAttribute< std::string >( "type" ) );

						auto effect = effectManager->Find( methodElement.GetAttribute< std::string >( "effect" ) );

						// Determine attributes used...
						if( methodElement.HasAttributes( "baseVertex" ) )
						{
							int baseVertex = methodElement.GetAttribute< int >( "basevertex" );
							int minIndex = methodElement.GetAttribute< int >( "minindex" );
							int vertices = methodElement.GetAttribute< int >( "vertices" );
							int startIndex = methodElement.GetAttribute< int >( "startIndex" );
							int primitives = methodElement.GetAttribute< int >( "primitives" );
							bool useIB = true;
							bufferset.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, baseVertex, minIndex, vertices, startIndex, primitives, effect, useIB ) );
						}
						else
						{
							int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
							int vertices = methodElement.GetAttribute< int >( "vertices" );
							int primitives = methodElement.GetAttribute< int >( "primitives" );
							bool useIB = true;
							bufferset.GetRenderMethodBuffer().AddMethod( RenderMethod( pt, startVertex, vertices, primitives, effect, useIB ) );
						}
					}
				}
			}
		}
	}
}