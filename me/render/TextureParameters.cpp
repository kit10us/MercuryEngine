// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/TextureParameters.h>

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

TextureParameters::TextureParameters( unify::Parameters parameters )
	: TextureParameters{}
{
	if( parameters.Exists( "min" ) )
	{
		min = Filtering::FromString( parameters.Get< std::string >( "min" ) );
	}
	if( parameters.Exists( "mag" ) )
	{
		mag = Filtering::FromString( parameters.Get< std::string >( "mag" ) );
	}
	if( parameters.Exists( "mip" ) )
	{
		mip = Filtering::FromString( parameters.Get< std::string >( "mip" ) );
	}
	if( parameters.Exists( "lockaccess.cpu" ) )
	{
		lockAccess.cpu = unify::DataLockAccess::FromString( parameters.Get< std::string >( "lockaccess.cpu" ) );
	}
	if( parameters.Exists( "lockaccess.gpu" ) )
	{
		lockAccess.gpu = unify::DataLockAccess::FromString( parameters.Get< std::string >( "lockaccess.gpu" ) );
	}
	if( parameters.Exists( "source" ) )
	{
		source = unify::Path( parameters.Get< std::string >( "source" ) );
	}
	if( parameters.Exists( "width" ) )
	{
		size.width = parameters.Get< unsigned int >( "width" );
	}
	if( parameters.Exists( "height" ) )
	{
		size.height = parameters.Get< unsigned int >( "height" );
	}
	if( parameters.Exists( "format" ) )
	{
		format = render::Format::FromString( parameters.Get< std::string >( "format" ) );
	}
}
