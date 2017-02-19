// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectOrbitMotivator.h>

using namespace me;
using namespace motivator;

ObjectOrbitMotivator::ObjectOrbitMotivator( unify::V3< float > origin, unify::V3< float > orbit, unify::Angle angleASecond )
	: m_origin{ origin }
	, m_orbit{ orbit }
	, m_angleASecond{ angleASecond }
{
}

ObjectOrbitMotivator::~ObjectOrbitMotivator() 
{
}

std::string ObjectOrbitMotivator::GetName() const
{
	return "ObjectOrbitMotivator";
}

bool ObjectOrbitMotivator::IsEnabled() const
{
	return m_enabled;
}

void ObjectOrbitMotivator::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool ObjectOrbitMotivator::Updateable() const
{
	return true;
}

bool ObjectOrbitMotivator::Renderable() const
{
	return false;
}

void ObjectOrbitMotivator::OnAttach( scene::Object * object )
{
	m_target = object;
}

void ObjectOrbitMotivator::OnDetach()
{
	m_target = nullptr;
}

void ObjectOrbitMotivator::OnInit()
{
}

void ObjectOrbitMotivator::OnStart()
{
}

void ObjectOrbitMotivator::OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo )
{
	if ( ! m_target )
	{
		return;
	}

	if ( ! m_enabled )
	{
		return;
	}

	m_target->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( m_orbit, m_angleASecond * renderInfo.GetDelta() ) );
	
	m_target->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}

void ObjectOrbitMotivator::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
}

void ObjectOrbitMotivator::OnSuspend()
{
}

void ObjectOrbitMotivator::OnResume()
{
}

scene::IObjectComponent * ObjectOrbitMotivator::Duplicate()
{
	auto duplicate = new ObjectOrbitMotivator( m_origin, m_orbit, m_angleASecond );
	return duplicate;
}

void ObjectOrbitMotivator::SetOrigin( unify::V3< float > origin )
{
	m_origin = origin;
}
			
void ObjectOrbitMotivator::SetOrbit( unify::V3< float > orbit )
{
	m_orbit = orbit;
}
			
void ObjectOrbitMotivator::SetAngleASecond( unify::Angle angleASecond )
{
	m_angleASecond = angleASecond;
}
			
unify::V3< float > ObjectOrbitMotivator::GetOrigin() const
{
	return m_origin;
}

unify::V3< float > ObjectOrbitMotivator::GetOrbit() const
{
	return m_orbit;
}

unify::Angle ObjectOrbitMotivator::GetAngleASecond() const
{
	return m_angleASecond;
}
