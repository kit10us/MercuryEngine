// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/ObjectActionComponent.h>

using namespace me;
using namespace object;

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

void ObjectActionComponent::OnUpdate(UpdateParams params)
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
	return me::object::IObjectComponent::ptr();
}

std::string ObjectActionComponent::GetWhat() const
{
	return std::string();
}

