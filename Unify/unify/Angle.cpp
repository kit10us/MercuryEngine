// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Angle.h>
#include <cmath>

using namespace unify;

Angle unify::AngleInRadians( float radians )
{
	return Angle( radians );
}

Angle unify::AngleInDegrees( float degrees )
{
	return Angle( degrees * (3.14159265f / 180.0f) );
}

Angle unify::AnglePI()
{
	return AngleInRadians( 3.14159265f );
}

Angle unify::AnglePI2()
{
	return AngleInRadians( 3.14159265f * 2.0f );
}

Angle unify::AnglePIHalf()
{
	return AngleInRadians( 3.14159265f * 0.5f );
}


Angle::Angle( float radians )
: m_radians( radians )
{
}

Angle::Angle()
: m_radians( 0.0f )
{
}

Angle::Angle( const Angle & angle )
	: m_radians( angle.m_radians )
{
}

Angle & Angle::operator = ( const Angle & angle )
{
    m_radians = angle.m_radians;
    return *this;
}

Angle & Angle::operator += ( const Angle & angle )
{
    m_radians += angle.m_radians;
    return *this;
}

Angle & Angle::operator -= ( const Angle & angle )
{
    m_radians -= angle.m_radians;
    return *this;
}

Angle & Angle::operator *= ( const Angle & angle )
{
    m_radians *= angle.m_radians;
    return *this;
}

Angle & Angle::operator /= ( const Angle & angle )
{
    m_radians /= angle.m_radians;
    return *this;
}

Angle Angle::operator + ( const Angle & angle) const
{
    Angle returnAngle( *this );
    returnAngle += angle;
    return returnAngle;
}

Angle Angle::operator - ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle -= angle;
    return returnAngle;
}

Angle Angle::operator * ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle *= angle;
    return returnAngle;
}

Angle Angle::operator / ( const Angle & angle ) const
{
    Angle returnAngle( *this );
    returnAngle /= angle;
    return returnAngle;
}

bool Angle::operator == ( const Angle & angle ) const
{
    return m_radians == angle.m_radians;
}

Angle & Angle::operator *= ( float scalar )
{
    m_radians *= scalar;
    return *this;
}

Angle & Angle::operator /= ( float scalar )
{
    m_radians /= scalar;
    return *this;
}

Angle Angle::operator * ( float scalar ) const
{
    Angle returnAngle( *this );
    returnAngle.m_radians *= scalar;
    return returnAngle;
}

Angle Angle::operator / ( float scalar ) const
{
    Angle returnAngle( *this );
    returnAngle.m_radians /= scalar;
    return returnAngle;
}

bool Angle::operator > ( const Angle & angle )
{
	return m_radians > angle.m_radians;
}

bool Angle::operator >= ( const Angle & angle )
{
	return m_radians >= angle.m_radians;
}

bool Angle::operator < ( const Angle & angle )
{
	return m_radians < angle.m_radians;
}

bool Angle::operator <= ( const Angle & angle )
{
	return m_radians <= angle.m_radians;
}

float Angle::ToRadians() const
{
    return m_radians;
}

float Angle::ToDegrees() const
{
    return m_radians * (3.14159265f / 180.0f);
}

int Angle::Fix360()
{
    const float pi2 = (3.14159265f * 2.0f);
	int magnitude = 0;
    while( m_radians > pi2 )
    {
        m_radians -= pi2;
		magnitude -= 1;
    }
    while( m_radians < 0 )
    {
        m_radians += pi2;
		magnitude += 1;
    }
	return magnitude;
}

float Angle::SinOf() const
{
	return (float)sin( m_radians );
}

float Angle::CosOf() const
{
	return (float)cos( m_radians );
}