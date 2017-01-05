// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/ControllerBase.h>

using namespace me;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

ControllerBase::ControllerBase( ControllerBase & controller )
	: m_os( controller.m_os )
	, m_sceneComponent( controller.m_sceneComponent )
{
}


ControllerBase::ControllerBase( me::IOS * os, SceneComponent * sceneComponent )
	: m_os( os )
	, m_sceneComponent( sceneComponent )
{
}

ControllerBase::~ControllerBase()
{
}

me::IOS * ControllerBase::GetOS()
{
	return m_os;
}

const me::IOS * ControllerBase::GetOS() const
{
	return m_os;
}

bool ControllerBase::IsEnabled() const
{
	return m_enabled;
}

void ControllerBase::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

PxController * ControllerBase::GetController()
{
	return m_controller.get();
}

const PxController * ControllerBase::GetController() const
{
	return m_controller.get();
}

DXIPHYSX_API SceneComponent * ControllerBase::GetScene()
{
	return m_sceneComponent;
}

DXIPHYSX_API const SceneComponent * ControllerBase::GetScene() const
{
	return m_sceneComponent;
}
