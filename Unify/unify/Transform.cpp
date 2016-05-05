// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/Transform.h>

using namespace unify;

Transform::Transform()
: m_position( 0, 0, 0 )
, m_scale( 1, 1, 1 )
, m_orientation( Quaternion::QuaternionIdentity() )
{
}

Transform::Transform( const Transform & transform )
: m_position( transform.m_position )
, m_scale( transform.m_scale )
, m_orientation( transform.m_orientation )
{
}

Transform::Transform( const Matrix & matrix )
: m_position( matrix.GetScale() )
, m_scale( matrix.GetScale() )
, m_orientation( matrix.GetRotation() )
{
}

Transform::Transform( const Quaternion orientation, const V3< float > position, const V3< float > scale )
: m_position( position )
, m_scale( scale )
, m_orientation( orientation )
{
}

Transform::~Transform()
{
}

const V3< float > & Transform::GetPosition() const
{
	return m_position;
}

const V3< float > & Transform::GetScale() const
{
	return m_scale;
}

const Quaternion & Transform::GetOrientation() const
{
	return m_orientation;
}

Matrix Transform::MakeMatrix() const
{
	return Matrix( m_orientation, m_position, m_scale );
}
