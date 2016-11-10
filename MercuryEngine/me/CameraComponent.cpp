// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/CameraComponent.h>

using namespace me;

CameraComponent::CameraComponent( CameraComponent & component )
	: ObjectComponent( component )
	, m_projection( component.m_projection )
	, m_renderer( component.m_renderer )
{
}

CameraComponent::CameraComponent( IOS * os )
	: ObjectComponent( os )
	, m_projection( unify::MatrixIdentity() )
	, m_renderer( 0 )
{
}

CameraComponent::~CameraComponent()
{
}

std::string CameraComponent::GetName() const
{
	return "Camera";
}

int CameraComponent::GetRenderer() const
{
	return m_renderer;
}

void CameraComponent::SetRenderer( int index )
{
	m_renderer = index;
}

void CameraComponent::SetProjection( const unify::Matrix & projection )
{
	m_projection = projection;
}

unify::Matrix CameraComponent::GetProjection() const
{
	return m_projection;
}

IObjectComponent * CameraComponent::Duplicate()
{
	auto duplicate = new CameraComponent( *this );
	return duplicate;
}
