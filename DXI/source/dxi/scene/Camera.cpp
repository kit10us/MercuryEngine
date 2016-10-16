// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Camera.h>

using namespace dxi;
using namespace scene;

Camera::Camera( core::IOS * os )
: Component( os, "Camera" )
, m_projection( unify::Matrix::MatrixIdentity() )
, m_renderer( 0 )
{
}

Camera::~Camera()
{
}

int Camera::GetRenderer() const
{
	return m_renderer;
}

void Camera::SetRenderer( int index )
{
	m_renderer = index;
}

void Camera::SetProjection( const unify::Matrix & projection )
{
	m_projection = projection;
}

unify::Matrix Camera::GetProjection() const
{
	return m_projection;
}

void Camera::OnInit( Object * object )
{
}

void Camera::OnStart( Object * object )
{
}

void Camera::Update( const RenderInfo & renderInfo )
{
}

void Camera::Render( const RenderInfo & renderInfo )
{
}

void Camera::OnSuspend()
{
}

void Camera::OnResune()
{
}
