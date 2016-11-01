// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/GeometryFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/NotImplemented.h>
#include <dxi/XMLConvert.h>
#include <qxml/Document.h>

using namespace dxi;

void LoadMesh_1_2( core::Game * game, const qxml::Element & geometryElement, dxi::Mesh * mesh );

GeometryFactory::GeometryFactory( core::IGame * game )
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
		auto game = dynamic_cast<core::Game *>(m_game);
		LoadMesh_1_2( game, geometryElement, mesh );
	}
	else
	{
		throw exception::FailedToCreate( "Geometry XML version " + version + " not supported!" );
	}

	mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );

	return Geometry::ptr( mesh );
}


void LoadMesh_1_2( core::Game * game, const qxml::Element & geometryElement, dxi::Mesh * mesh )
{
	// Managers to store sub-resources.
	auto textureManager = game->GetManager< Texture >();
	auto pixelShaderManager = game->GetManager< PixelShader >();
	auto vertexShaderManager = game->GetManager< VertexShader >();
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
			BufferSet & bufferset = mesh->GetPrimitiveList().AddBufferSet();

			for( auto buffersetChild : child.Children() )
			{
				if( buffersetChild.IsTagName( "vertexlist" ) )
				{	
					std::string effectName = buffersetChild.GetAttribute< std::string >( "effect" );
					auto effect = effectManager->Find( effectName );
					VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
					unsigned int vertexCount = buffersetChild.GetAttribute< unsigned int >( "count" );
					
					VertexBuffer & vb = bufferset.GetVertexBuffer();
					std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );

					unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

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
							vd->WriteVertex( lock, index, positionE, xyz );
							vb.GetBBox() += xyz;
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
							vd->WriteVertex( lock, index, normalE, normal );
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
							vd->WriteVertex( lock, index, texE, uv );
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
							vd->WriteVertex( lock, index, texE2, uv2 );
						}
	
						unify::ColorUnit diffuseUnit;
						if( XMLConvert( &vertex, diffuseUnit ) )
						{
							unify::Color diffuse( diffuseUnit );
							vd->WriteVertex( lock, index, diffuseE, diffuse );
						}
					}

					vb.Create( vertexCount, vd, 0, vertices.get() );

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

					IndexBuffer & ib = bufferset.GetIndexBuffer();
					ib.Create( numIndices, (Index32*)&indices[0], dxi::BufferUsage::Default );
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
							bufferset.GetRenderMethodBuffer().AddMethod( method );
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
							bufferset.GetRenderMethodBuffer().AddMethod( method );
							break;
						}
						case PrimitiveType::TriangleList:
							if ( methodElement.HasAttributes( "indices" ) == false )
							{
								int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
								int triangleCount = methodElement.GetAttribute< int >( "trianglecount" );
								RenderMethod method( RenderMethod::CreateTriangleList( startVertex, triangleCount, effect ));
								bufferset.GetRenderMethodBuffer().AddMethod( method );
							}
							else
							{
								int vertices = methodElement.GetAttribute< int >( "vertices" );
								int indices = methodElement.GetAttribute< int >( "indices" );
								int startIndex = methodElement.GetAttributeElse < int >("startindex", 0);
								int baseVertexIndex = methodElement.GetAttributeElse< int >( "baseVertexIndex", 0 );
								RenderMethod method( RenderMethod::CreateTriangleListIndexed( vertices, indices, startIndex, baseVertexIndex, effect ) );
								bufferset.GetRenderMethodBuffer().AddMethod( method );
							}
							break;
						case PrimitiveType::TriangleStrip:
							if ( methodElement.HasAttributes( "indices" ) == false )
							{
								int startVertex = methodElement.GetAttributeElse< int >( "startvertex", 0 );
								int segmentCount = methodElement.GetAttribute< int >( "segmentcount" );
								RenderMethod method( RenderMethod::CreateTriangleStrip( startVertex, segmentCount, effect ) );
								bufferset.GetRenderMethodBuffer().AddMethod( method );
							}
							else
							{
								int vertices = methodElement.GetAttribute< int >( "vertices" );
								int indices = methodElement.GetAttribute< int >( "indices" );
								int startIndex = methodElement.GetAttributeElse < int >( "startindex", 0 );
								int baseVertexIndex = methodElement.GetAttributeElse< int >( "baseVertexIndex", 0 );
								RenderMethod method( RenderMethod::CreateTriangleStripIndexed( vertices, indices, startIndex, baseVertexIndex, effect ) );
								bufferset.GetRenderMethodBuffer().AddMethod( method );
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