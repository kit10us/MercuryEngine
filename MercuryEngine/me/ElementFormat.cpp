// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/ElementFormat.h>

using namespace me;

size_t ElementFormat::SizeOf( ElementFormat::TYPE format )
{
	switch ( format )
	{
	case ElementFormat::Float1:
		return sizeof( float );
	case ElementFormat::Float2:
		return sizeof( float ) * 2;
	case ElementFormat::Float3:
		return sizeof( float ) * 3;
	case ElementFormat::Float4:
		return sizeof( float ) * 4;
	case ElementFormat::Matrix4x4:
		return sizeof( float ) * 4 * 4;
	case ElementFormat::ColorUNorm:
		return sizeof( unify::Color );
	case ElementFormat::Unknown:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}

template< typename T >
T Convert( std::string in, std::initializer_list< std::pair< std::string, T > > pairs, bool( *compare )(const std::string &, const std::string &) = unify::StringIs )
{
	for ( auto && pair : pairs )
	{
		if ( compare( in, pair.first ) ) return pair.second;
	}
	throw unify::Exception( "Conversion of " + in + " not found!" );
}

ElementFormat::TYPE ElementFormat::FromString( std::string format )
{
	return Convert< ElementFormat::TYPE>( format,
	{
		{ "Float1", ElementFormat::Float1 },
		{ "Float2", ElementFormat::Float2 },
		{ "TexCoord", ElementFormat::Float2 },
		{ "Float3", ElementFormat::Float3 },
		{ "Float4", ElementFormat::Float4 },
		{ "Matrix4x4", ElementFormat::Matrix4x4 },
		{ "ColorUNorm", ElementFormat::ColorUNorm },
		{ "Color", ElementFormat::ColorUNorm }
	} );
}

std::string ElementFormat::ToString( ElementFormat::TYPE format )
{
	switch ( format )
	{
	case ElementFormat::Float1:
		return "Float1";
	case ElementFormat::Float2:
		return "Float2";
	case ElementFormat::Float3:
		return "Float3";
	case ElementFormat::Float4:
		return "Float4";
	case ElementFormat::Matrix4x4:
		return "Matrix4x4";
	case ElementFormat::ColorUNorm:
		return "ColorUNorm";
	case ElementFormat::Unknown:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}
