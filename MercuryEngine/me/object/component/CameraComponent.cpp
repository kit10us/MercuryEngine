// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/object/component/CameraComponent.h>

using namespace me;
using namespace object;
using namespace component;

CameraComponent::CameraComponent( CameraComponent & component )
	: ObjectComponent( component )
	, m_projection( component.m_projection )
	, m_renderer( component.m_renderer )
{
}

CameraComponent::CameraComponent()
	: ObjectComponent( "Camera", false, true )
	, m_projection( unify::MatrixIdentity() )
	, m_renderer( 0 )
{
}

CameraComponent::~CameraComponent()
{
}

std::string CameraComponent::GetWhat() const
{
	return std::string( "render " ) + unify::Cast< std::string >( m_render );
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

IObjectComponent::ptr CameraComponent::Duplicate()
{
	auto duplicate = new CameraComponent( *this );
	return IObjectComponent::ptr( duplicate );
}
