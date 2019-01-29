// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/ITexture.h>

using namespace me;
using namespace render;

TextureParameters::TextureParameters()
	: size{}
	, lockAccess{ unify::DataLockAccess::None, unify::DataLockAccess::Readonly }
	, min( Filtering::Linear )
	, mag( Filtering::Linear )
	, mip( Filtering::Linear )
	, format{ me::render::Format::R32G32B32A32_Float }
	, usage{ me::render::BufferUsage::Default }
{
}

TextureParameters::TextureParameters( unify::Path _source, me::render::TextureLockAccess lockAccess )
	: source( _source )
	, lockAccess{ lockAccess }
	, min( Filtering::Linear )
	, mag( Filtering::Linear )
	, mip( Filtering::Linear )
	, format{ me::render::Format::R32G32B32A32_Float }
	, usage{ me::render::BufferUsage::Default }
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
	if( element->HasAttributes( "lockaccescpu" ) )
	{
		lockAccess.cpu = element->GetAttribute< unify::DataLockAccess::TYPE >( "lockaccesscpu" );
	}
	if( element->HasAttributes( "lockaccessgpu" ) )
	{
		lockAccess.gpu = element->GetAttribute< unify::DataLockAccess::TYPE >( "lockaccessgpu" );
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
