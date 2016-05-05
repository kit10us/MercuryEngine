// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Quaternion.h>
#include <unify/Matrix.h>

using namespace unify;

Quaternion Quaternion::QuaternionIdentity()
{
	Quaternion q;
#ifdef DIRECTX9
	D3DXQuaternionIdentity( (D3DXQUATERNION*)&q.m_quaternion );
#else
	q.m_quaternion = DirectX::XMQuaternionIdentity().m128_f32;
#endif
	return q;
}

Quaternion Quaternion::QuaternionFromEuler( const unify::V3< float > & euler )
{
	Quaternion quaternion;
#ifdef DIRECTX9
	D3DXQuaternionRotationYawPitchRoll( (D3DXQUATERNION*)&quaternion.m_quaternion, euler.y, euler.x, euler.y );
#else
	quaternion.m_quaternion = DirectX::XMQuaternionRotationRollPitchYaw( euler.x, euler.y, euler.z ).m128_f32;
#endif
	return quaternion;
}

Quaternion Quaternion::QuaternionRotationAxis( const V3< float > & axis, float angle )
{
	Quaternion quaternion( Quaternion::QuaternionIdentity() );
#ifdef DIRECTX9
	D3DXVECTOR3 axisD3D( axis.x, axis.y, axis.z );
	D3DXQuaternionRotationAxis( (D3DXQUATERNION*)&quaternion.m_quaternion, &axisD3D, angle );
#else
	DirectX::XMVECTORF32 axisD3D;
	axisD3D.f[ 0 ] = axis.x;
	axisD3D.f[ 1 ] = axis.y;
	axisD3D.f[ 2 ] = axis.z;
	axisD3D.f[ 3 ] = 0.0f;
	quaternion.m_quaternion = DirectX::XMQuaternionRotationAxis( axisD3D, angle ).m128_f32;
#endif
	return quaternion;
}

Quaternion Quaternion::QuaternionSlerp( const Quaternion & quaternionA, const Quaternion & quaternionB, float mix )
{
	Quaternion resultQuaternion;
#ifdef DIRECTX9
	D3DXQuaternionSlerp( (D3DXQUATERNION*)&resultQuaternion.m_quaternion, (D3DXQUATERNION*)&quaternionA.m_quaternion, (D3DXQUATERNION*)&quaternionB.m_quaternion, mix );
#else
	resultQuaternion.m_quaternion = DirectX::XMQuaternionSlerp( quaternionA.GetD3DXQuaternion(), quaternionB.GetD3DXQuaternion(), mix ).m128_f32;
#endif
	return resultQuaternion;
}


Quaternion::Quaternion()
// No default effort.
{
}

Quaternion::Quaternion( float x, float y, float z, float w )
: m_quaternion( x, y, z, w )
{
}

Quaternion::Quaternion( const Quaternion & quaternion )
: m_quaternion( quaternion.m_quaternion )
{
}

Quaternion::Quaternion( const Matrix & matrix )
{
#ifdef DIRECTX9
	D3DXQuaternionRotationMatrix( (D3DXQUATERNION*)&m_quaternion, matrix.GetD3DXMatrix() );
#else
	m_quaternion = DirectX::XMQuaternionRotationMatrix( *matrix.GetD3DXMatrix() ).m128_f32;
#endif
}

// assignment operators
Quaternion & Quaternion::operator += ( const Quaternion & quaternion )
{
	m_quaternion += quaternion.m_quaternion;
	return *this;
}

Quaternion & Quaternion::operator -= ( const Quaternion & quaternion )
{
	m_quaternion -= quaternion.m_quaternion;
	return *this;
}

Quaternion & Quaternion::operator *= ( const Quaternion & quaternion )
{
	m_quaternion *= quaternion.m_quaternion;
	return *this;
}

Quaternion & Quaternion::operator *= ( float value )
{
	m_quaternion *= value;
	return *this;
}

Quaternion & Quaternion::operator /= ( float value )
{
	m_quaternion /= value;
	return *this;
}

Quaternion Quaternion::operator + ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut += quaternion;
}

Quaternion Quaternion::operator - ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut -= quaternion;
}

Quaternion Quaternion::operator * ( const Quaternion & quaternion ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut *= quaternion;
}

Quaternion Quaternion::operator * ( float value ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut *= value;
}

Quaternion Quaternion::operator / ( float value ) const
{
	Quaternion quaternionOut( *this );
	return quaternionOut /= value;
}

bool Quaternion::operator == ( const Quaternion & quaternion ) const
{
	return m_quaternion == quaternion.m_quaternion ? true : false;
}

bool Quaternion::operator != ( const Quaternion & quaternion ) const
{
	return m_quaternion != quaternion.m_quaternion ? true : false;
}

void Quaternion::SetX( float x )
{
	m_quaternion.x = x;
}

void Quaternion::SetY( float y )
{
	m_quaternion.y = y;
}

void Quaternion::SetZ( float z )
{
	m_quaternion.z = z;
}

void Quaternion::SetW( float w )
{
	m_quaternion.w = w;
}

float Quaternion::GetX() const
{
	return m_quaternion.x;
}

float Quaternion::GetY() const
{
	return m_quaternion.y;
}

float Quaternion::GetZ() const
{
	return m_quaternion.z;
}

float Quaternion::GetW() const
{
	return m_quaternion.w;
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion( GetX() * -1, GetY() * -1, GetZ() * -1, GetW() );
}

void Quaternion::TransformVector( V3< float > & vector ) const
{
	float length = vector.Normalize();
	Quaternion vecQuaternion( vector.x, vector.y, vector.z, 0 );
	Quaternion resQuaternion( vecQuaternion * Conjugate() );
	resQuaternion = *this * resQuaternion;
	vector.x = resQuaternion.GetX();
	vector.y = resQuaternion.GetY();
	vector.z = resQuaternion.GetZ();
	vector *= length;
}

Quaternion::DXQuaternion Quaternion::GetD3DXQuaternion() const
{
#ifdef DIRECTX9
	D3DXQUATERNION q( (float*)&m_quaternion );
#else
	DXQuaternion qdx;
	q.m128_f32[ 0 ] = m_quaternion[ 0 ];
	q.m128_f32[ 1 ] = m_quaternion[ 1 ];
	q.m128_f32[ 2 ] = m_quaternion[ 2 ];
	q.m128_f32[ 3 ] = m_quaternion[ 3 ];
#endif
	return q;
}
