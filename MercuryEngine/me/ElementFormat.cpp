// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/ElementFormat.h>
#include <unify/ColorUnit.h>
#include <unify/Matrix.h>

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

ElementFormat::TYPE ElementFormat::FromString( std::string format )
{
	if ( unify::StringIs( format, "Float1" ) ) return ElementFormat::Float1;
	else if ( unify::StringIs( format, "Float2" ) ) return ElementFormat::Float2;
	else if ( unify::StringIs( format, "TexCoord" ) ) return ElementFormat::Float2;
	else if ( unify::StringIs( format, "Float3" ) ) return ElementFormat::Float3;
	else if ( unify::StringIs( format, "Float4" ) ) return ElementFormat::Float4;
	else if ( unify::StringIs( format, "Matrix4x4" ) ) return ElementFormat::Matrix4x4;
	else if ( unify::StringIs( format, "ColorUNorm" ) ) return ElementFormat::ColorUNorm;
	else if ( unify::StringIs( format, "Color" ) ) return ElementFormat::ColorUNorm;
	else
	{
		throw unify::Exception( "Conversion of " + format + " not found!" );
	}
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

bool ElementFormat::Convert( TYPE outFormat, void * outRaw, TYPE inFormat, const void * inRaw )
{
	size_t inCount = 0;
	size_t outCount = 0;
	switch( inFormat )
	{
	case ElementFormat::Float4:
		inCount++;
	case ElementFormat::Float3:
		inCount++;
	case ElementFormat::Float2:
		inCount++;
	case ElementFormat::Float1:
		inCount++;

		switch ( outFormat )
		{
		case ElementFormat::Float4:
			outCount++;
		case ElementFormat::Float3:
			outCount++;
		case ElementFormat::Float2:
			outCount++;
		case ElementFormat::Float1:
			outCount++;

			{
				for ( size_t i = 0; i < outCount; i++ )
				{
					((float*)outRaw)[i] = i < inCount ? ((const float*)inRaw)[i] : 0.0f;
				}
			}
			return true;

		case ElementFormat::ColorUNorm:
		{
			unify::ColorUnit cu;

			for ( size_t i = 0; i < 4; i++ )
			{
				cu.linear[i] = i < inCount ? ((const float*)inRaw)[i] : 0.0f;
			}

			*((unify::Color*)outRaw) = cu;
			return true;
		}
		default:
			return false;
		}
				
	case ElementFormat::ColorUNorm:
	{
		const unify::Color c( *((const unify::Color*)inRaw) );
		switch ( outFormat )
		{
		case ElementFormat::Float3:
		{
			const unify::ColorUnit cu( c );
			unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>(outRaw);
			destItem->x = cu.component.r;
			destItem->y = cu.component.g;
			destItem->z = cu.component.b;
			return true;
		}
		case ElementFormat::Float4:
		{
			const unify::ColorUnit cu( c );
			unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>(outRaw);
			destItem->x = cu.component.r;
			destItem->y = cu.component.g;
			destItem->z = cu.component.b;
			destItem->w = cu.component.a;
			return true;
		}
		case ElementFormat::ColorUNorm:
		{
			unify::Color * destItem = reinterpret_cast<unify::Color *>(outRaw);
			*destItem = c;
			return true;
		}
		default:
			return false;
		}
	}

	case ElementFormat::Matrix4x4:
		switch ( outFormat )
		{
		case ElementFormat::Matrix4x4:
			*((unify::Matrix*)outRaw) = *((const unify::Matrix*)inRaw);
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
}