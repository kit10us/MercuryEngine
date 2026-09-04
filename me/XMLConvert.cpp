// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/XMLConvert.h>
#include <unify/color.h>

bool me::XMLConvert( const qxml::Element * element, unify::V3< float > & v3, std::string postFix )
{
	if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix ) )
	{
		v3.x = element->GetAttribute( "x" + postFix )->Get< float >();
		v3.y = element->GetAttribute( "y" + postFix )->Get< float >();
		v3.z = element->GetAttribute( "z" + postFix )->Get< float >();
	}
	else if( element->HasElements( "x" + postFix + ",y" + postFix + ",z" + postFix ) )
	{
		auto x = unify::FromString< float >( element->GetElement( "x" + postFix )->GetText() );
		auto y = unify::FromString< float >( element->GetElement( "y" + postFix )->GetText() );
		auto z = unify::FromString< float >( element->GetElement( "z" + postFix )->GetText() );

		if (!x || !y || !z)
		{
			return false;
		}

		v3.x = *x;
		v3.y = *y;
		v3.z = *z;
	}
	else
	{
		return false;
	}
	return true;
}

bool me::XMLConvert( const qxml::Element * element, unify::Quaternion & q, std::string postFix )
{
	// Rotation about an axis...
	if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix ) )
	{
		unify::V3< float > v( element->GetAttribute( "x" + postFix )->Get< float >(), element->GetAttribute( "y" + postFix )->Get< float >(), element->GetAttribute( "z" + postFix )->Get< float >() );
		unify::Angle rotation( unify::AngleInRadians( element->GetAttribute( "rotation" + postFix )->Get< float >() ) );
		q = unify::Quaternion( v, rotation );
	}
	else if( element->HasElements( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix ) )
	{
		unify::V3< float > v;
		auto x = unify::FromString<float>( element->GetElement( "x" + postFix )->GetText() );
		auto y = unify::FromString<float>( element->GetElement( "y" + postFix )->GetText() );
		auto z = unify::FromString<float>( element->GetElement( "z" + postFix )->GetText() );
		
		if (!x || !y || !z)
		{
			return false;
		}

		v.x = *x;
		v.y = *y;
		v.z = *z;

		unify::Angle rotation( unify::AngleInRadians( element->GetAttribute( "rotation" + postFix )->Get< float >() ) );
		q = unify::Quaternion( v, rotation );
	}

	// Explicit quaternion
	else if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix + ",w" + postFix ) )
	{
		q.x = ( element->GetAttribute( "x" + postFix )->Get< float >() );
		q.y = ( element->GetAttribute( "y" + postFix )->Get< float >() );
		q.z = ( element->GetAttribute( "z" + postFix )->Get< float >() );
		q.w = ( element->GetAttribute( "w" + postFix )->Get< float >() );
	}
	else if( element->HasElements( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix + ",w" + postFix ) )
	{
		auto x = ( unify::FromString< float >( element->GetElement( "x" + postFix )->GetText() ) );
		auto y = ( unify::FromString< float >( element->GetElement( "y" + postFix )->GetText() ) );
		auto z = ( unify::FromString< float >( element->GetElement( "z" + postFix )->GetText() ) );
		auto w = ( unify::FromString< float >( element->GetElement( "w" + postFix )->GetText() ) );

		if (!x || !y || !z || !w)
		{
			return false;
		}

		q.x = *x;
		q.y = *y;
		q.z = *z;
		q.w = *w;
	}
	else
	{
		return false;
	}
	return true;
}

bool me::XMLConvert( const qxml::Element * element, unify::Matrix & matrix, std::string postFix )
{
	unify::V3< float > scale( 1, 1, 1 );
	unify::Quaternion rotation( unify::QuaternionIdentity() );
	unify::V3< float > translation( 0, 0, 0 );
	int finds = 0;

	if( element->HasElements( "scale" + postFix ) )
	{
		XMLConvert( element->GetElement( "scale" + postFix ), scale );
		++finds;
	}
	if( element->HasElements( "position" + postFix ) )
	{
		XMLConvert( element->GetElement( "position" + postFix ), translation );
		++finds;
	}
	if( element->HasElements( "translation" + postFix ) )
	{
		XMLConvert( element->GetElement( "translation" + postFix ), translation );
		++finds;
	}
	if( element->HasElements( "rotation" + postFix ) )
	{
		XMLConvert( element->GetElement( "rotation" + postFix ), rotation );
		++finds;
	}

	if( finds )
	{
		matrix = unify::Matrix( rotation, translation );
		matrix *= unify::MatrixScale( scale );
		return true;
	}
	else
	{
		return false;
	}
}

bool me::XMLConvert( const qxml::Element * element, unify::TexCoords & texCoords, std::string postFix )
{
	if( element->HasAttributes( "u" + postFix + ",v" + postFix ) )
	{
		texCoords.u = element->GetAttribute( "u" + postFix )->Get< float >();
		texCoords.v = element->GetAttribute( "v" + postFix )->Get< float >();
	}
	else if( element->HasElements( "u" + postFix + ",v" + postFix ) )
	{
		auto u = unify::FromString< float >( element->GetElement( "u" + postFix )->GetText() );
		auto v = unify::FromString< float >( element->GetElement( "v" + postFix )->GetText() );

		if (!u || !v)
		{
			return false;
		}

		texCoords.u = *u;
		texCoords.v = *v;
	}
	else
	{
		return false;
	}
	return true;
}

bool me::XMLConvert( const qxml::Element * element, unify::ColorUnit & color, std::string postFix )
{
	if( element->HasAttributes( "red" + postFix + ",green" + postFix + ",blue" + postFix ) )
	{
		color.SetRed( element->GetAttribute( "red" + postFix )->Get< float >() );
		color.SetGreen( element->GetAttribute( "green" + postFix )->Get< float >() );
		color.SetBlue( element->GetAttribute( "blue" + postFix )->Get< float >() );
		if( element->HasAttributes( "alpha" ) )
		{
			color.SetAlpha( element->GetAttribute( "alpha" + postFix )->Get< float >() );
		}
	}
	else if( element->HasElements( "red" + postFix + ",green" + postFix + ",blue" + postFix ) )
	{
		auto r = unify::FromString< float >( element->GetElement( "red" + postFix )->GetText() );
		auto g = unify::FromString< float >( element->GetElement( "green" + postFix )->GetText() );
		auto b = unify::FromString< float >( element->GetElement( "blue" + postFix )->GetText() );
		std::optional<float> a;
		if( element->HasElements( "alpha" + postFix ) )
		{
			a = unify::FromString< float >( element->GetElement( "alpha" + postFix )->GetText() );
		}

		color.SetRed( *r );
		color.SetGreen( *g );
		color.SetBlue( *b );

		if( element->HasElements( "alpha" + postFix ) )
		{
			color.SetAlpha( *a );
		}
	}
	else
	{
		return false;
	}
	return true;
}
