// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Camera.h>

using namespace dxi;
using namespace scene;

Camera::Camera()
{
}

Camera::Camera( Object::shared_ptr object )
: m_object( object )
, m_projection( unify::Matrix::MatrixIdentity() )
{
}

Camera::~Camera()
{
}

void Camera::SetObject( Object::shared_ptr object )
{
	m_object = object;
}

bool Camera::HasObject() const
{
	return ! m_object.expired();
}

Object::shared_ptr Camera::GetObject()
{
    return m_object.lock();
}

void Camera::SetProjection( const unify::Matrix & projection )
{
	m_projection = projection;
}

unify::Matrix Camera::GetProjection() const
{
	return m_projection;
}

unify::Matrix Camera::GetMatrix() const
{
	if( ! m_object.expired() )
	{
		return m_object.lock().get()->GetFrame().GetMatrix();
	}

    return unify::Matrix::MatrixIdentity();
}

void Camera::LookAt( const unify::V3< float > & at )
{
	GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
}


