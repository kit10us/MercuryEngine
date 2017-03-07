// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectInputMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

ObjectInputMotivator::ObjectInputMotivator( IOS * os )
	: ObjectComponent( os )
{
}

ObjectInputMotivator::~ObjectInputMotivator() 
{
}

bool ObjectInputMotivator::Updateable() const
{
	return true;
}

bool ObjectInputMotivator::Renderable() const
{
	return false;
}

void ObjectInputMotivator::OnAttach( scene::Object * object )
{
	m_target = object;
}

void ObjectInputMotivator::OnDetach()
{
	m_target = nullptr;
}

void ObjectInputMotivator::OnInit()
{
}

void ObjectInputMotivator::OnStart()
{
}

void ObjectInputMotivator::OnUpdate( UpdateParams params )
{
}

void ObjectInputMotivator::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
}

void ObjectInputMotivator::OnSuspend()
{
}

void ObjectInputMotivator::OnResume()
{
}

scene::Object * ObjectInputMotivator::GetTarget() const
{
	return m_target;
}

motivator::IInputMotivator * ObjectInputMotivator::GetMotivator()
{
	return &m_motivator;
}
