// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/XMLConvert.h>
#include <unify/color.h>

bool dxi::XMLConvert( const qxml::Element * element, unify::V3< float > & v3, const std::string & postFix )
{
	if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix ) )
	{
		v3.x = element->GetFloatAttribute( "x" + postFix );
		v3.y = element->GetFloatAttribute( "y" + postFix );
		v3.z = element->GetFloatAttribute( "z" + postFix );
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

bool dxi::XMLConvert( const qxml::Element * element, unify::Quaternion & q, const std::string & postFix )
{
	// Rotation about an axis...
	if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix ) )
	{
		unify::V3< float > v( element->GetFloatAttribute( "x" + postFix ), element->GetFloatAttribute( "y" + postFix ), element->GetFloatAttribute( "z" + postFix ) );
		float rotation = element->GetFloatAttribute( "rotation" + postFix );
		q = unify::Quaternion::QuaternionRotationAxis( v, rotation );
	}
	else if( element->HasElements( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix ) )
	{
		unify::V3< float > v;
		v.x = unify::Cast< float >( element->GetElement( "x" + postFix )->GetText() );
		v.y = unify::Cast< float >( element->GetElement( "y" + postFix )->GetText() );
		v.z = unify::Cast< float >( element->GetElement( "z" + postFix )->GetText() );
		float rotation = unify::Cast< float >( element->GetElement( "rotation" + postFix )->GetText() );
		q = unify::Quaternion::QuaternionRotationAxis( v, rotation );
	}

	// Explicit quaternion
	else if( element->HasAttributes( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix + ",w" + postFix ) )
	{
		q.SetX( element->GetFloatAttribute( "x" + postFix ) );
		q.SetY( element->GetFloatAttribute( "y" + postFix ) );
		q.SetZ( element->GetFloatAttribute( "z" + postFix ) );
		q.SetW( element->GetFloatAttribute( "w" + postFix ) );
	}
	else if( element->HasElements( "x" + postFix + ",y" + postFix + ",z" + postFix + ",rotation" + postFix + ",w" + postFix ) )
	{
		q.SetX( unify::Cast< float >( element->GetElement( "x" + postFix )->GetText() ) );
		q.SetY( unify::Cast< float >( element->GetElement( "y" + postFix )->GetText() ) );
		q.SetZ( unify::Cast< float >( element->GetElement( "z" + postFix )->GetText() ) );
		q.SetW( unify::Cast< float >( element->GetElement( "w" + postFix )->GetText() ) );
	}
	else
	{
		return false;
	}
	return true;
}

bool dxi::XMLConvert( const qxml::Element * element, unify::Matrix & matrix, const std::string & postFix )
{
	unify::V3< float > scale( 1, 1, 1 );
	unify::Quaternion rotation( unify::Quaternion::QuaternionIdentity() );
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
		matrix = unify::Matrix( rotation, translation, scale );
		return true;
	}
	else
	{
		return false;
	}
}

bool dxi::XMLConvert( const qxml::Element * element, unify::TexCoords & texCoords, const std::string & postFix )
{
	if( element->HasAttributes( "u" + postFix + ",v" + postFix ) )
	{
		texCoords.u = element->GetFloatAttribute( "u" + postFix );
		texCoords.v = element->GetFloatAttribute( "v" + postFix );
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

bool dxi::XMLConvert( const qxml::Element * element, unify::ColorUnit & color, const std::string & postFix )
{
	if( element->HasAttributes( "red" + postFix + ",green" + postFix + ",blue" + postFix ) )
	{
		color.SetRed( element->GetFloatAttribute( "red" + postFix ) );
		color.SetGreen( element->GetFloatAttribute( "green" + postFix ) );
		color.SetBlue( element->GetFloatAttribute( "blue" + postFix ) );
		if( element->HasAttributes( "alpha" ) )
		{
			color.SetAlpha( element->GetFloatAttribute( "alpha" + postFix ) );
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
