// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#include <ase/GeometryFactory.h>
#include <ase/ASEDocument.h>
#include <dxi/exception/FailedToCreate.h>
#include <unify/String.h>
#include <unify/FrameSet.h>
#include <unify/ColorUnit.h>

using namespace ase;
using namespace dxi;

GeometryFactory::GeometryFactory( dxi::core::Game * game )
: m_game( game )
{
}

void GeometryFactory::SetVertexShader( dxi::VertexShader::ptr vertexShader )
{
	m_vertexShader = vertexShader;
}

void GeometryFactory::SetPixelShader( dxi::PixelShader::ptr pixelShader )
{
	m_pixelShader = pixelShader;
}

Geometry * GeometryFactory::Produce( unify::Path source )
{
	core::Game & game = *m_game;

	// Managers to store sub-resources.
	auto textureManager = game.GetManager< Texture >();
	auto pixelShaderManager = game.GetManager< PixelShader >();
	auto vertexShaderManager = game.GetManager< VertexShader >();
	auto effectManager = game.GetManager< Effect >();

	Mesh * mesh = new Mesh;
	PrimitiveList & primitiveList = mesh->GetPrimitiveList();

	ase::Document doc;
	Texture::ptr texture;
	unsigned int u = 0;
	
	doc.Load( source );	  

	// Mapping of materialsx to effects.
	std::vector< std::shared_ptr< Effect > > materialList;

	for( auto node : doc.GetRoot()->Children() )
	{
		if ( node.IsTagName( "MATERIAL_LIST" ) )
		{
			for( auto material : node.Children( "MATERIAL" ) )
			{
				std::shared_ptr< Effect > effect{ new Effect };
				materialList.push_back( effect );

				effect->SetVertexShader( m_vertexShader );
				effect->SetPixelShader( m_pixelShader );
				//material.GetElement( "MATERIAL_NAME" )->GetText()

				/*
				qxml::Element * ambient = material.GetElement( "MATERIAL_AMBIENT" );
				unify::ColorUnit ambient = unify::ColorUnit::ColorUnitARGB( 1, ambient->GetAttribute< float >( "R" ), ambient->GetAttribute< float >( "G" ), ambient->GetAttribute< float >( "B" ) );
				qxml::Element * diffuse = material.GetElement( "MATERIAL_DIFFUSE" );
				unify::ColorUnit diffuse = unify::ColorUnit::ColorUnitARGB( 1, diffuse->GetAttribute< float >( "R" ), diffuse->GetAttribute< float >( "G" ), diffuse->GetAttribute< float >( "B" ) );
				qxml::Element * specular = material.GetElement( "MATERIAL_SPECULAR" );
				unify::ColorUnit specular = unify::ColorUnit::ColorUnitARGB( 1, specular->GetAttribute< float >( "R" ), specular->GetAttribute< float >( "G" ), specular->GetAttribute< float >( "B" ) );
				qxml::Element * shine = material.GetElement( "MATERIAL_SHINE" );

				effect.GetMaterial().SetAmbient( ambient );
				effect.GetMaterial().SetDiffuse( diffuse );
				effect.GetMaterial().SetSpecular( specular );
				*/

				// Texture...
				qxml::Element * mapDiffuseElement = material.GetElement( "MAP_DIFFUSE" );
				//std::string sMapName = pMap->GetElement( "MAP_NAME" )->GetText();
				if( mapDiffuseElement != nullptr )
				{
					qxml::Element * bitmapElement = mapDiffuseElement->GetElement( "BITMAP" );
					unify::Path texturePath = source.DirectoryOnly() + 	bitmapElement->GetText();
					Texture::ptr texture = textureManager->Add( bitmapElement->GetText(), texturePath );
					effect->SetTexture( 0, texture );
				}
			}
		}

		if ( node.IsTagName( "GEOMOBJECT" ) )
		{
			// Grab the material index (directly indexes the effect list) so we can get the VD...
			qxml::Element * materialRef = node.GetElement( "MATERIAL_REF" );
			assert( materialRef ); // TODO: Presumably this could be null, which likely means the default material - this is not support yet.

			unsigned int effectIndex = 0;
			effectIndex = unify::Cast< unsigned int >( materialRef->GetText() );
			Effect::ptr effect = materialList[effectIndex];

			for( auto child : node.Children() )
			{
				if ( child.IsTagName( "MESH" ) )
				{
					qxml::Element & mesh = child;

					BufferSet & bufferSet = primitiveList.AddBufferSet();

					// Handle vertex format...
					const VertexDeclaration::ptr & vd = effect->GetVertexShader()->GetVertexDeclaration();

					unsigned short stream = 0;

					VertexElement positionE = CommonVertexElement::Position( stream );
					VertexElement normalE = CommonVertexElement::Normal( stream );
					VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
					VertexElement specularE = CommonVertexElement::Specular( stream );
					VertexElement texE = CommonVertexElement::TexCoords( stream );
					
					unsigned int mesh_numfaces = unify::Cast< unsigned int >( mesh.GetElement( "MESH_NUMFACES" )->GetText() );
					unsigned int uNumPVertices; // Positional "MESH_VERTEX"
					unsigned int uNumTVertices; // Texture "MESH_TVERT"

					uNumPVertices = unify::Cast< unsigned int >( mesh.GetElement( "MESH_NUMVERTEX" )->GetText() );
					uNumTVertices = unify::Cast< unsigned int >( mesh.GetElement( "MESH_NUMTVERTEX" )->GetText() );

					struct Face
					{	
						int mesh_face_A, mesh_face_B, mesh_face_C;
						int mesh_face_AB, mesh_face_BC, mesh_face_CA;
						int mesh_tface_A, mesh_tface_B, mesh_tface_C;

						int normal_A, normal_B, normal_C;
					};
					
					std::vector< Face > faces( mesh_numfaces );

					const qxml::Element * mesh_face_list = mesh.GetElement( "MESH_FACE_LIST" );
					for( auto mesh_face : mesh_face_list->Children( "MESH_FACE" ) )
					{
						size_t index = mesh_face.GetAttribute< size_t >( "index" );
						faces[index].mesh_face_A = mesh_face.GetAttribute< int >( "A" );
						faces[index].mesh_face_B = mesh_face.GetAttribute< int >( "B" );
						faces[index].mesh_face_C = mesh_face.GetAttribute< int >( "C" );
						faces[index].mesh_face_AB = mesh_face.GetAttribute< int >( "AB" );
						faces[index].mesh_face_BC = mesh_face.GetAttribute< int >( "BC" );
						faces[index].mesh_face_CA = mesh_face.GetAttribute< int >( "CA" );
					}

					//qxml::ElementList listFace;
					//mesh.FindElementsByTagNameRecursive( listFace, "MESH_FACE" );

					const qxml::Element * mesh_tface_list = mesh.GetElement( "MESH_TFACELIST" );
					for( auto mesh_tface : mesh_tface_list->Children( "MESH_TFACE" ) )
					{
						size_t index = mesh_tface.GetAttribute< size_t >( "index" );
						faces[index].mesh_tface_A = mesh_tface.GetAttribute< int >( "A" );
						faces[index].mesh_tface_B = mesh_tface.GetAttribute< int >( "B" );
						faces[index].mesh_tface_C = mesh_tface.GetAttribute< int >( "C" );
					}								 

					// Position vertex index, Texture coord index, actual index.
					// Since the model has only unique positions and textures (two buffers really), and 
					// we are storing the position and the texture together, we will assume every face has unqiue vertices and textures.
					// This will likely produce WAY more vertices then necessary, however, this can be mitigated using utility functions.
					struct PositionTexturePairing 
					{
						int iP;
						int iT;
					};

					// Load position and texture indices...
					std::vector< PositionTexturePairing > listPTP( mesh_numfaces * 3 );
					for( u = 0; u < mesh_numfaces; u++ )
					{
						listPTP.push_back( { faces[u].mesh_face_B, faces[u].mesh_tface_B } );
						listPTP.push_back( { faces[u].mesh_face_C, faces[u].mesh_tface_C } );
					}
	
					// Create the vertex buffer for position and normals...
					std::vector< unify::V3< float > > positions( uNumPVertices );

					// Grab positional and normal information...
					qxml::Element * mesh_vertex_list = mesh.GetElement( "MESH_VERTEX_LIST" );
					for( auto mesh_vertex : mesh_vertex_list->Children( "MESH_VERTEX" ) )
					{
						int index = mesh_vertex.GetAttribute< int >( "index" );
						positions[ index ].x = mesh_vertex.GetAttribute< float >( "x" );
						positions[ index ].y = mesh_vertex.GetAttribute< float >( "y" );
						positions[ index ].z = mesh_vertex.GetAttribute< float >( "z" );
					}

					qxml::Element * mesh_tvertexList = mesh.GetElement( "MESH_TVERTLIST" );					
					std::vector< unify::TexCoords > texCoords( unify::Cast< size_t >( mesh.GetElement( "MESH_NUMTVERTEX" )->GetText() ) );
					for( auto mesh_tvert : mesh_tvertexList->Children( "MESH_TVERT" ) )
					{
						int index = mesh_tvert.GetAttribute< int >( "index" );
						texCoords[ index ].u = mesh_tvert.GetAttribute< float >( "u" );
						texCoords[ index ].v = 1.0f - mesh_tvert.GetAttribute< float >( "v" );
					}

					// Create vertex and index buffer...
					VertexBuffer & vb = bufferSet.GetVertexBuffer();
					
					std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * listPTP.size()] );

					unify::DataLock lock( vertices.get(), vd->GetSize(), listPTP.size(), false );

					std::vector< Index32 > indices( (unsigned int)listPTP.size() * 3 );

					const qxml::Element * mesh_normals = mesh.GetElement( "MESH_NORMALS" );																					
					for( const auto mesh_facenormal : mesh_normals->Children( "MESH_FACENORMAL" ) )
					{
						size_t index = mesh_facenormal.GetAttribute< size_t >( "index" );

						vd->WriteVertex( lock, index * 3 + 0, positionE, positions[faces[index].mesh_face_A] );
						vd->WriteVertex( lock, index * 3 + 1, positionE, positions[faces[index].mesh_face_B] );
						vd->WriteVertex( lock, index * 3 + 2, positionE, positions[faces[index].mesh_face_C] );

						vd->WriteVertex( lock, index * 3 + 0, texE, texCoords[faces[index].mesh_tface_A] );
						vd->WriteVertex( lock, index * 3 + 1, texE, texCoords[faces[index].mesh_tface_B] );
						vd->WriteVertex( lock, index * 3 + 2, texE, texCoords[faces[index].mesh_tface_C] );

						size_t normalIndex = 0;
						for( const auto mesh_vertexnormal : mesh_facenormal.Children( "MESH_VERTEXNORMAL" ) )
						{
							unify::V3< float > vNormal{ qxml::AttributeCast< unify::V3< float >, float >( mesh_vertexnormal, "x", "y", "z" ) };
							vd->WriteVertex( lock, index * 3 + normalIndex++, normalE, vNormal );
						}						

						indices[index * 3 + 0] = (Index32)index * 3 + 0;
						indices[index * 3 + 1] = (Index32)index * 3 + 1;
						indices[index * 3 + 2] = (Index32)index * 3 + 2;
					}
					
					vb.Create( (unsigned int)listPTP.size(), vd, vertices.get() );

					IndexBuffer & ib = bufferSet.GetIndexBuffer();
					ib.Create( (unsigned int)listPTP.size() * 3, (Index32*)&indices[0], dxi::BufferUsage::Default );

					bufferSet.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, mesh_numfaces * 3, mesh_numfaces, effect, true /*useIB*/ ) );
				}
			}
		}
	}

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );
	return mesh;
}
