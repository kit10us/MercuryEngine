// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/object/Object.h>
#include <me/object/component/CameraComponent.h>
#include <me/object/component/TagsComponent.h>

using namespace me;
using namespace scene;
using namespace object;

Object::Object()
	: m_name{}
	, m_alive{ false }
	, m_scene{}
	, m_enabled{ true }
	, m_components{}
	, m_frame{}
{
	AddComponent( component::IObjectComponent::ptr{ new component::TagsComponent() } );
}

Object::Object( Object && objectFrom )
{
	m_alive = objectFrom.m_alive;
	m_name = objectFrom.m_name;
	m_scene = objectFrom.m_scene;
	m_enabled = objectFrom.m_enabled;
	m_components = objectFrom.m_components;
	m_frame = objectFrom.m_frame;
}

Object::~Object()
{
}

void Object::CopyFrom( std::string name, Object & objectFrom )
{
	m_name = name;
	m_enabled = objectFrom.m_enabled;
	m_frame = objectFrom.m_frame;
	m_frame = objectFrom.GetFrame();

	for( auto component : objectFrom.m_components )
	{
		AddComponent( component::IObjectComponent::ptr( component.Component()->Duplicate() ) );
	}
}

void Object::SetAllocator( scene::IObjectAllocator* allocator )
{
	m_allocator = allocator;
}

void Object::SetName( std::string name )
{
	m_name = name;
}

std::string Object::GetName() const
{
	return m_name;
}

int Object::GetComponentCount() const
{
	return (int)m_components.size();
}

void Object::AddComponent( component::IObjectComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( ComponentInstance< component::IObjectComponent::ptr >( component ) );
}

void Object::RemoveComponent( component::IObjectComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

component::IObjectComponent::ptr Object::GetComponent( int index )
{
	if ( index > (int)m_components.size() )
	{
		return nullptr;
	}

	int i = 0;
	for( auto component : m_components )
	{
		if( index == i ) return component.Component();
		++i;
	}

	return nullptr;
}

component::IObjectComponent::ptr Object::GetComponent( std::string typeName, std::string alias )
{
	int componentIndex = FindComponent( typeName, alias );
	return GetComponent( componentIndex );
}
  
int Object::FindComponent( std::string typeName, std::string alias ) const
{
	int i = 0;
	for( auto && component : m_components )
	{
		if( unify::StringIs( component.Component()->GetTypeName(), typeName ) )
		{
			if( alias.empty() )
			{
				return i;
			}
			else if( unify::StringIs( component.Component()->GetAlias(), alias ) )
			{
				return i;
			}
		}
		++i;
	}		
	return -1;
}

void Object::ClearComponents()
{
	for( auto & itr : m_components )
	{
		itr.Component()->OnDetach( this );
	}
	m_components.clear();
}

void Object::SetEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Object::IsEnabled() const
{
    return m_enabled;
}

bool Object::IsUpdateable() const
{
	for( auto && component : m_components )
	{
		if ( component.Component()->Updateable() ) return true;
	}		
	return false;
}

unify::FrameLite & Object::GetFrame()
{
	return m_frame;
}

const unify::FrameLite & Object::GetFrame() const
{
	return m_frame;
}

unify::V3< float > Object::GetPosition() const
{
	return m_frame.GetPosition();
}

unify::BBox< float > Object::GetBBox() const
{
	unify::BBox< float > bbox;

	// Update components...
	for( auto && component : m_components )
	{
		// Only start and update if enabled.
		if( ! component.Component()->IsEnabled() ) continue;

		component.Component()->GetBBox( bbox, GetFrame().GetMatrix() );
	}
	return bbox;
}

unify::BSphere< float > Object::GetBSphere() const
{
	return GetBBox().MakeBSphere();
}

bool Object::Intersects( unify::Ray ray ) const
{
	return GetBBox().Intersects( ray );
}

bool Object::Intersects( unify::Ray ray, float distanceBegin, float distanceEnd ) const
{
	return GetBBox().Intersects( ray, distanceBegin, distanceEnd );
}

bool Object::Intersects( unify::Ray ray, unify::V3< float > & hitPoint ) const
{
	return GetBBox().Intersects( ray, hitPoint );
}

bool Object::Intersects( unify::Ray ray, float & distance ) const
{
	return GetBBox().Intersects( ray, distance );
}

void Object::Initialize( component::IObjectComponent::cache & updateables, CameraCache & cameras, UpdateParams params )
{
	// Update components...
	for( auto && component : m_components )
	{
		// Only start and update if enabled.
		if ( !component.Component()->IsEnabled() ) continue;

		// Start is basically a way to get us into a beginning state.
		if ( component.IsStarted() == false )
		{
			component.Component()->OnStart();
			component.SetStarted( true );
		}

		// Updateables...
		if ( component.Component()->Updateable() )
		{
			updateables.push_back( component.Component().get() );
		}

		// Check for a camera...
		component::CameraComponent * camera{};
		camera = dynamic_cast< component::CameraComponent * >(component.Component().get());
		if( camera != nullptr )
		{
			cameras.push_back( FinalCamera{ this, camera } );
		}
	}
}

void Object::CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans )
{
	for( auto && component : m_components )
	{
		component.Component()->CollectGeometry( solids, trans, &m_frame );
	}
}

void Object::OnSuspend()
{
	for( auto && component : m_components )
	{
		if( component.Component()->IsEnabled() )
		{
			component.Component()->OnSuspend();
		}
	}
}

void Object::OnResume()
{
	for( auto && component : m_components )
	{
		if( component.Component()->IsEnabled() )
		{
			component.Component()->OnResume();
		}
	}
}

void Object::SetScene( IScene * scene )
{
	m_scene = scene;
}

IScene * Object::GetScene()
{
	return m_scene;
}

bool Object::IsAlive() const
{
	return m_alive;
}

void Object::SetAlive( bool alive )
{
	m_alive = alive;
}

void Object::MakeDirty()
{
	m_allocator->DirtyObject( this );
}
