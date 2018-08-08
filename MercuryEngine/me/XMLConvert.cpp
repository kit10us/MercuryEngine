// Copyright (c) 2002 - 2018, Evil Quail LLC
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
		v3.x = unify::Cast< float >( element->GetElement( "x" + postFix )->GetText() );
		v3.y = unify::Cast< float >( element->GetElement( "y" + postFix )->GetText() );
		v3.z = unify::Cast< float >( element->GetElement( "z" + postFix )->GetText() );
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
		v.x = unify::Cast< float >( element->GetElement( "x" + postFix )->GetText() );
		v.y = unify::Cast< float >( element->GetElement( "y" + postFix )->GetText() );
		v.z = unify::Cast< float >( element->GetElement( "z" + postFix )->GetText() );
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
		q.x = ( unify::Cast< float >( element->GetElement( "x" + postFix )->GetText() ) );
		q.y = ( unify::Cast< float >( element->GetElement( "y" + postFix )->GetText() ) );
		q.z = ( unify::Cast< float >( element->GetElement( "z" + postFix )->GetText() ) );
		q.w = ( unify::Cast< float >( element->GetElement( "w" + postFix )->GetText() ) );
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
		texCoords.u = unify::Cast< float >( element->GetElement( "u" + postFix )->GetText() );
		texCoords.v = unify::Cast< float >( element->GetElement( "v" + postFix )->GetText() );
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
		color.SetRed( unify::Cast< float >( element->GetElement( "red" + postFix )->GetText() ) );
		color.SetGreen( unify::Cast< float >( element->GetElement( "green" + postFix )->GetText() ) );
		color.SetBlue( unify::Cast< float >( element->GetElement( "blue" + postFix )->GetText() ) );
		if( element->HasElements( "alpha" + postFix ) )
		{
			color.SetAlpha( unify::Cast< float >( element->GetElement( "alpha" + postFix )->GetText() ) );
		}
	}
	else
	{
		return false;
	}
	return true;
}
