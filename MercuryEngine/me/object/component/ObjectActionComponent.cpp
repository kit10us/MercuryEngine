// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/object/component/ObjectActionComponent.h>

using namespace me;
using namespace object;
using namespace component;

ObjectActionComponent::ObjectActionComponent( ObjectActionComponent & component )
	: ObjectComponent( component )
{
}

ObjectActionComponent::ObjectActionComponent( action::IObjectAction::ptr action )
	: ObjectComponent( "ObjectActionComponent", true, false )
	, m_action{ action }
	, m_stopped{ false }
{	
}

ObjectActionComponent::~ObjectActionComponent()
{
}

void ObjectActionComponent::OnUpdate( const UpdateParams & params)
{
	if (m_stopped)
	{
		return;
	}

	if (m_action)
	{
		m_stopped = !m_action->Perform( GetObject(), params.GetDelta());
	}
}

IObjectComponent::ptr ObjectActionComponent::Duplicate()
{
	return IObjectComponent::ptr();
}

std::string ObjectActionComponent::GetWhat() const
{
	return std::string();
}

