// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <sg/ShapeFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/ITexture.h>
#include <qxml/Document.h>

using namespace me;
using namespace sg;

ShapeFactory::ShapeFactory( IGame * game )
	: m_game( game )
{
}

Geometry::ptr ShapeFactory::Produce( unify::Path source, void * data )
{	
	auto game = dynamic_cast< Game *>(m_game);

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
				params.Set( p.GetTagName(), Shape::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "effect" ) )
			{
				params.Set( p.GetTagName(), game->GetManager< Effect >()->Add( p.GetAttribute< std::string >( "name" ), doc.GetPath().DirectoryOnly() + p.GetAttribute< std::string >( "source" ) ) );
			}	 
			else if ( p.IsTagName( "inf" ) || p.IsTagName( "sup" ) || p.IsTagName( "center" ) || p.IsTagName( "offset" ) )
			{
				params.Set( p.GetTagName(), unify::V3< float >( p.GetText() ) );
			}
			//else if ( p.IsTagName( " std::vector< unify::Color > diffuses = parameters.Get< std::vector< unify::Color > >( "diffuses", std::vector< unify::Color >() );
			else if ( p.IsTagName( "count" ) )
			{
				params.Set( p.GetTagName(), unify::Cast< unsigned int >( p.GetText() ) );
			}
			else if ( p.IsTagName( "diffuse" ) || p.IsTagName( "specular" ) )
			{
				params.Set( p.GetTagName(), unify::Color( p.GetText() ) );
			}
			else if( p.IsTagName( "segments" ) )
			{
				params.Set( p.GetTagName(), unify::Cast< unsigned int >( p.GetText() ) );
			}
			else if( p.IsTagName( "size3" ) )
			{
				params.Set( p.GetTagName(), unify::Size3< float >( p.GetText() ) );
			}
			else if( p.IsTagName( "texturemode" ) )
			{
				params.Set( p.GetTagName(), TextureMode::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "radius" ) || p.IsTagName( "majorradius" ) || p.IsTagName( "minorradius" ) || p.IsTagName( "definition" ) || p.IsTagName( "size1" ) || p.IsTagName( "height" ) )
			{
				params.Set( p.GetTagName(), unify::Cast< float >( p.GetText() ) );
			}
			else if ( p.IsTagName( "bufferusage" ) )
			{
				params.Set( p.GetTagName(), BufferUsage::FromString( p.GetText() ) );
			}
			else if( p.IsTagName( "texarea" ) )
			{
				params.Set( p.GetTagName(), unify::TexArea( p.GetText() ) );
			}
			else if( p.IsTagName( "caps" ) )
			{
				params.Set( p.GetTagName(), unify::Cast< bool >( p.GetText() ) );
			}
		}
		shape = CreateShape( m_game->GetOS()->GetRenderer( 0 ), params );
	}
	catch( std::exception ex )
	{
		game->ReportError( ErrorLevel::Failure, "Shape Factory", ex.what() );
	}

	
	return shape;
}

