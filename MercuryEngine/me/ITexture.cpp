// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/ITexture.h>

using namespace me;
TextureParameters::TextureParameters()
	: renderable( true )
	, lockable( false )
	, min( me::Filtering::Linear )
	, mag( me::Filtering::Linear )
	, mip( me::Filtering::Linear )
{
}

TextureParameters::TextureParameters( unify::Path _source, bool _renderable, bool _lockable )
	: source( _source )
	, renderable( _renderable )
	, lockable( _lockable )
	, min( me::Filtering::Linear )
	, mag( me::Filtering::Linear )
	, mip( me::Filtering::Linear )
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

	source = unify::Path( element->GetAttribute< std::string >( "source" ) );
}
