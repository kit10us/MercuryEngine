// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>
#include <dxi/Texture.h>

using namespace dxi;

Geometry * ShapeXMLFactory::Produce( const qxml::Element & element )
{	
	auto game = core::Game::GetInstance();

	unify::Parameters params;

	for( auto p : element.Children() )
	{
		if( p.GetType() != qxml::Element::NodeType::Element ) continue;

		if( p.IsTagName( "type" ) )
		{
			params.Set( p.GetTagName(), shapes::Shape::FromString( p.GetText() ) );
		}
		else if( p.IsTagName( "effect" ) )
		{
			params.Set( p.GetTagName(), game->GetManager< Effect >()->Add( p ) );
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
	return shapes::CreateShape( params );
}

