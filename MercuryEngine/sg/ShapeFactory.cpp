// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <sg/ShapeFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/render/ITexture.h>
#include <qxml/Document.h>

using namespace me;
using namespace sg;

ShapeFactory::ShapeFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}

render::Geometry::ptr ShapeFactory::Produce( unify::Path source, void * data )
{	
	using namespace render;

	auto gameInstance = dynamic_cast< game::Game *>(m_game);

	Mesh::ptr shape;

	try
	{
		qxml::Document doc( source );
	
		auto & element = *doc.GetRoot()->FindFirstElement( "shape" );

		unify::Parameters params;

		for( auto p : element.Children() )
		{
			if( p.GetType() != qxml::Element::NodeType::Element ) continue;

			if( p.IsTagName( "type" ) )
			{
				params.Set( p.GetName(), Shape::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "effect" ) )
			{
				params.Set( p.GetName(), gameInstance->GetManager< Effect >()->Add( p.GetAttribute< std::string >( "name" ), p.GetAttribute< unify::Path >( "source" ), doc.GetPath().DirectoryOnly() ) );
			}	 
			else if ( p.IsTagName( "inf" ) || p.IsTagName( "sup" ) || p.IsTagName( "center" ) || p.IsTagName( "offset" ) )
			{
				params.Set( p.GetName(), unify::V3< float >( p.GetText() ) );
			}
			//else if ( p.IsTagName( " std::vector< unify::Color > diffuses = parameters.Get< std::vector< unify::Color > >( "diffuses", std::vector< unify::Color >() );
			else if ( p.IsTagName( "count" ) )
			{
				params.Set( p.GetName(), unify::Cast< unsigned int >( p.GetText() ) );
			}
			else if ( p.IsTagName( "diffuse" ) || p.IsTagName( "specular" ) )
			{
				params.Set( p.GetName(), unify::Color( p.GetText() ) );
			}
			else if( p.IsTagName( "segments" ) )
			{
				params.Set( p.GetName(), unify::Cast< unsigned int >( p.GetText() ) );
			}
			else if( p.IsTagName( "size3" ) )
			{
				params.Set( p.GetName(), unify::Size3< float >( p.GetText() ) );
			}
			else if( p.IsTagName( "texturemode" ) )
			{
				params.Set( p.GetName(), TextureMode::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "radius" ) || p.IsTagName( "majorradius" ) || p.IsTagName( "minorradius" ) || p.IsTagName( "definition" ) || p.IsTagName( "size1" ) || p.IsTagName( "height" ) )
			{
				params.Set( p.GetName(), unify::Cast< float >( p.GetText() ) );
			}
			else if ( p.IsTagName( "bufferusage" ) )
			{
				params.Set( p.GetName(), BufferUsage::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "texarea" ) )
			{
				params.Set( p.GetName(), unify::TexArea( p.GetText() ) );
			}
			else if( p.IsTagName( "caps" ) )
			{
				params.Set( p.GetName(), unify::Cast< bool >( p.GetText() ) );
			}
		}
		shape = CreateShape( m_game->GetOS()->GetRenderer( 0 ), params );
	}
	catch( std::exception ex )
	{
		gameInstance->Debug()->ReportError( ErrorLevel::Failure, "Shape Factory", ex.what() );
	}

	
	return shape;
}

