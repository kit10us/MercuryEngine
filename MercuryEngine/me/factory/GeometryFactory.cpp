// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/factory/GeometryFactory.h>
#include <me/VertexUtil.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/NotImplemented.h>
#include <me/XMLConvert.h>
#include <qxml/Document.h>

using namespace me;

void LoadMesh_1_2( Game * game, const qxml::Element & geometryElement, Mesh * mesh );

GeometryFactory::GeometryFactory( IGame * game )
	: m_game( game )
{
}

Geometry::ptr GeometryFactory::Produce( unify::Path source, void * data )
{
	qxml::Document doc( source );
	auto & geometryElement = *doc.GetRoot()->FindFirstElement( "geometry" );

	if ( ! geometryElement.HasAttributes( "version" ) )
	{
		return 0;
	}

	Mesh * mesh = new Mesh( m_game->GetOS()->GetRenderer(0) );

	std::string version{ geometryElement.GetAttribute< std::string >( "version" ) };
	if( version == "1.2" )
	{
		auto game = dynamic_cast< Game * >(m_game);
		LoadMesh_1_2( game, geometryElement, mesh );
	}
	else
	{
		throw exception::FailedToCreate( "Geometry XML version " + version + " not supported!" );
	}

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );

	return Geometry::ptr( mesh );
}


void LoadMesh_1_2( Game * game, const qxml::Element & geometryElement, Mesh * mesh )
{
	// Managers to store sub-resources.
	auto textureManager = game->GetManager< ITexture >();
	auto pixelShaderManager = game->GetManager< IPixelShader >();
	auto vertexShaderManager = game->GetManager< IVertexShader >();
	auto effectManager = game->GetManager< Effect >();

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
			BufferSet & set = mesh->GetPrimitiveList().AddBufferSet();

			for( auto buffersetChild : child.Children() )
			{
				if( buffersetChild.IsTagName( "vertexlist" ) )
				{	
					std::string effectName = buffersetChild.GetAttribute< std::string >( "effect" );
					auto effect = effectManager->Find( effectName );
					IVertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
					unsigned int vertexCount = buffersetChild.GetAttribute< unsigned int >( "count" );
					
					std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize( 0 ) * vertexCount] );

					unify::DataLock lock( vertices.get(), vd->GetSize( 0 ), vertexCount, false, 0 );
					unify::BBox< float > bbox;

					unsigned short stream = 0;

					VertexElement positionE = CommonVertexElement::Position( stream );
					VertexElement normalE = CommonVertexElement::Normal( stream );
					VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
					VertexElement specularE = CommonVertexElement::Specular( stream );
					VertexElement texE = CommonVertexElement::TexCoords( stream );
					VertexElement texE2 = CommonVertexElement::TexCoords2( stream );

					for( auto vertex : buffersetChild.Children( "vertex" ) )
					{  		
						size_t index = vertex.GetAttribute< size_t >( "index" );
	
						// XYZ
						unify::V3< float > xyz;
						if( XMLConvert( &vertex, xyz ) )
						{
							WriteVertex( *vd, lock, index, positionE, xyz );
							bbox += xyz;
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
							WriteVertex( *vd, lock, index, normalE, normal );
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
							WriteVertex( *vd, lock, index, texE, uv );
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
							WriteVertex( *vd, lock, index, texE2, uv2 );
						}
	
						unify::ColorUnit diffuseUnit;
						if( XMLConvert( &vertex, diffuseUnit ) )
						{
							unify::Color diffuse( diffuseUnit );
							WriteVertex( *vd, lock, index, diffuseE, diffuse );
						}
					}

					set.AddVertexBuffer( { vertexCount, vd, 0, vertices.get(), BufferUsage::Default,  bbox } );

				}	 
				else if( buffersetChild.IsTagName( "indexlist" ) )
				{
					unsigned int numIndices = unify::ListPartCount( buffersetChild.GetText(), {','} );

					std::vector< Index32 > indices( numIndices );

					// Load indices...
					for( unsigned int u = 0; u < numIndices; u++ )
					{
						indices[u] = (Index32)unify::Cast< int >( unify::ListPart( buffersetChild.GetText(), {','}, u ) );
					}

					set.AddIndexBuffer( { numIndices, (Index32*)&indices[0], BufferUsage::Default } );
				}
				else if( buffersetChild.IsTagName( "methodlist" ) )
				{
					for( auto methodElement : buffersetChild.Children( "addmethod" ) )
					{
						// Determine Primitive Type...
						PrimitiveType::TYPE pt;
						pt = PrimitiveType::StringToPrimitiveType( methodElement.GetAttribute< std::string >( "type" ) );

						auto effect = effectManager->Find( methodElement.GetAttribute< std::string >( "effect" ) );


						switch( pt )
						{
						case PrimitiveType::LineList:
						{
							int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
							int lineCount = methodElement.GetAttribute< int >( "linecount" );
							RenderMethod method( RenderMethod::CreateLineList( startVertex, lineCount, effect ) );
							set.GetRenderMethodBuffer().AddMethod( method );
							break;
						}
						case PrimitiveType::LineStrip:
							throw exception::NotImplemented( "Line strip primitive type not implemented!" );
							break;
						case PrimitiveType::PointList:
						{
							int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
							int pointCount = methodElement.GetAttribute< int >( "pointcount" );
							RenderMethod method( RenderMethod::CreatePointList( startVertex, pointCount, effect ) );
							set.GetRenderMethodBuffer().AddMethod( method );
							break;
						}
						case PrimitiveType::TriangleList:
							if ( methodElement.HasAttributes( "indices" ) == false )
							{
								int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
								int triangleCount = methodElement.GetAttribute< int >( "trianglecount" );
								RenderMethod method( RenderMethod::CreateTriangleList( startVertex, triangleCount, effect ));
								set.GetRenderMethodBuffer().AddMethod( method );
							}
							else
							{
								int vertices = methodElement.GetAttribute< int >( "vertices" );
								int indices = methodElement.GetAttribute< int >( "indices" );
								int startIndex = methodElement.GetAttributeElse < int >("startindex", 0);
								int baseVertexIndex = methodElement.GetAttributeElse< int >( "baseVertexIndex", 0 );
								RenderMethod method( RenderMethod::CreateTriangleListIndexed( vertices, indices, startIndex, baseVertexIndex, effect ) );
								set.GetRenderMethodBuffer().AddMethod( method );
							}
							break;
						case PrimitiveType::TriangleStrip:
							if ( methodElement.HasAttributes( "indices" ) == false )
							{
								int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
								int segmentCount = methodElement.GetAttribute< int >( "segmentcount" );
								RenderMethod method( RenderMethod::CreateTriangleStrip( startVertex, segmentCount, effect ) );
								set.GetRenderMethodBuffer().AddMethod( method );
							}
							else
							{
								int vertices = methodElement.GetAttribute< int >( "vertices" );
								int indices = methodElement.GetAttribute< int >( "indices" );
								int startIndex = methodElement.GetAttributeElse < int >( "startindex", 0 );
								int baseVertexIndex = methodElement.GetAttributeElse< int >( "baseVertexIndex", 0 );
								RenderMethod method( RenderMethod::CreateTriangleStripIndexed( vertices, indices, startIndex, baseVertexIndex, effect ) );
								set.GetRenderMethodBuffer().AddMethod( method );
							}							break;
						default:
							throw exception::FailedToCreate( "Invalid primitive type!" );
						}
					}
				}
			}
		}
	}
}