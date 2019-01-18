// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/ITexture.h>

using namespace me;
using namespace render;

TextureParameters::TextureParameters()
	: size{}
	, renderable( true )
	, lockable( false )
	, min( Filtering::Linear )
	, mag( Filtering::Linear )
	, mip( Filtering::Linear )
	, format{ me::render::Format::R32G32B32A32_Float }
	, usage{ me::render::BufferUsage::Dynamic }
{
}

TextureParameters::TextureParameters( unify::Path _source, bool _renderable, bool _lockable )
	: source( _source )
	, renderable( _renderable )
	, lockable( _lockable )
	, min( Filtering::Linear )
	, mag( Filtering::Linear )
	, mip( Filtering::Linear )
	, format{ me::render::Format::R32G32B32A32_Float }
	, usage{ me::render::BufferUsage::Dynamic }
{
}

TextureParameters::TextureParameters( const qxml::Element * element )
	: TextureParameters{}
{
	if( element->HasAttributes( "min" ) )
	{
		min = Filtering::FromString( element->GetAttribute< std::string >( "min" ) );
	}
	if( element->HasAttributes( "mag" ) )
	{
		mag = Filtering::FromString( element->GetAttribute< std::string >( "mag" ) );
	}
	if( element->HasAttributes( "mip" ) )
	{
		mip = Filtering::FromString( element->GetAttribute< std::string >( "mip" ) );
	}
	if( element->HasAttributes( "lockable" ) )
	{
		lockable = element->GetAttribute< bool >( "lockable" );
	}
	if( element->HasAttributes( "renderable" ) )
	{
		renderable = element->GetAttribute< bool >( "renderable" );
	}
	if( element->HasAttributes( "source" ) )
	{
		source = unify::Path( element->GetAttribute< std::string >( "source" ) );
	}
	if ( element->HasAttributes( "width" ) )
	{
		size.width = element->GetAttribute< unsigned int >( "width" );
	}
	if (element->HasAttributes( "height" ) )
	{
		size.height = element->GetAttribute< unsigned int >( "height" );
	}
}
