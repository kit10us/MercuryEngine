// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/ControllerBase.h>

using namespace me;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

ControllerBase::ControllerBase( ControllerBase & controller )
	:  me::object::ObjectComponent( controller.GetTypeName() )
	, m_sceneComponent( controller.m_sceneComponent )
{
}
	
ControllerBase::ControllerBase( std::string typeName, SceneComponent * sceneComponent )
	: me::object::ObjectComponent( typeName )
	, m_sceneComponent( sceneComponent )
{
}

ControllerBase::~ControllerBase()
{
}

PxController * ControllerBase::GetController()
{
	return m_controller.get();
}

const PxController * ControllerBase::GetController() const
{
	return m_controller.get();
}

MEPHYSX_API SceneComponent * ControllerBase::GetScene()
{
	return m_sceneComponent;
}

MEPHYSX_API const SceneComponent * ControllerBase::GetScene() const
{
	return m_sceneComponent;
}
