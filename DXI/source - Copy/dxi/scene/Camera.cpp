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
, m_useProjection( false )
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
	m_useProjection = true;
}

void Camera::ClearProjection()
{
	m_projection = unify::Matrix::MatrixIdentity();
	m_useProjection = false;
}

unify::Matrix Camera::GetMatrix() const
{
	unify::Matrix finalMatrix = unify::Matrix::MatrixIdentity();
	if( ! m_object.expired() )
	{
		finalMatrix *= m_object.lock().get()->GetFrame().GetFinalMatrix();
		finalMatrix.Invert();
	}

	if ( m_useProjection )
	{
		finalMatrix *= m_projection;
	}

    return finalMatrix;
}

void Camera::LookAt( const unify::V3< float > & at )
{
	GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
	/*/
	const unify::Matrix & localMatrix = GetObject()->GetFrame().GetFinalMatrix();
	unify::V3< float > up( 0, 1, 0 );
	unify::V3< float > position( localMatrix.GetPosition() );
	unify::V3< float > forward( at - localMatrix.GetPosition() );
	forward.Normalize();

	unify::V3< float > left( unify::V3< float >::V3Cross( forward, up ) );

	unify::Matrix modelMatrix = unify::Matrix::MatrixIdentity();
	modelMatrix.SetForward( forward );
	modelMatrix.SetUp( up );
	modelMatrix.SetLeft( left );
	up = unify::V3< float >::V3Cross( forward, left );
	modelMatrix.SetUp( up ); // Regenerate up.
	modelMatrix.SetPosition( position );
	// TODO: remove parent frames effect, if we have one (inverse parent's local).
	GetObject()->GetFrame().SetModelMatrix( modelMatrix );
	*/
}


