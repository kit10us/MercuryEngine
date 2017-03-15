// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/object/Object.h>
#include <me/object/CameraComponent.h>

using namespace me;
using namespace scene;
using namespace object;

Object::Object()
	: m_name{}
	, m_alive{ false }
	, m_scene{}
	, m_enabled{ true }
	, m_tags{}
	, m_components{}
	, m_frame{}
{
}

Object::Object( Object && objectFrom )
{
	m_alive = objectFrom.m_alive;
	m_name = objectFrom.m_name;
	m_scene = objectFrom.m_scene;
	m_enabled = objectFrom.m_enabled;
	m_tags = objectFrom.m_tags;
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
	m_tags = objectFrom.m_tags;
	m_frame = objectFrom.GetFrame();

	for( auto component : objectFrom.m_components )
	{
		AddComponent( IObjectComponent::ptr( component.Component()->Duplicate() ) );
	}
}

void Object::SetName( std::string name )
{
	m_name = name;
}

std::string Object::GetName() const
{
	return m_name;
}

void Object::AddTag( std::string tag )
{
	m_tags.push_back( tag );
	m_tags.sort();
}

bool Object::HasTag( std::string tag ) const
{	
	for( auto && item : m_tags )
	{		  
		if ( unify::StringIs( item, tag ) ) return true;
	}
	return false;
}

int Object::GetComponentCount() const
{
	return (int)m_components.size();
}

void Object::AddComponent( IObjectComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( ComponentInstance< IObjectComponent::ptr >( component ) );
}

void Object::RemoveComponent( IObjectComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

IObjectComponent::ptr Object::GetComponent( int index )
{
	if ( index > (int)m_components.size() )
	{
		return IObjectComponent::ptr();
	}

	int i = 0;
	for( auto component : m_components )
	{
		if( index == i ) return component.Component();
		++i;
	}

	return IObjectComponent::ptr(); // Should never hit here.
}

IObjectComponent::ptr Object::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if( index == -1 ) return IObjectComponent::ptr();
	return GetComponent( index );
}
	  
int Object::FindComponent( std::string typeName, int startIndex ) const
{
	int i = 0;
	for( auto && component : m_components )
	{
		if( i >= startIndex && unify::StringIs( component.Component()->GetTypeName(), typeName ) ) return i;
		++i;
	}		
	return -1;
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

void Object::Initialize( IObjectComponent::cache & updateables, CameraCache & cameras, UpdateParams params )
{
	// Update components...
	for( auto && component : m_components )
	{
		// Regardless of enabled, ensure OnInit is always called.
		if ( component.IsInitialized() == false )
		{
			component.Component()->OnInit();
			component.SetInitialized( true );
		}

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
		CameraComponent * camera{};
		camera = dynamic_cast< CameraComponent * >(component.Component().get());
		if( camera != nullptr )
		{
			cameras.push_back( FinalCamera{ this, camera } );
		}
	}
}

void Object::CollectGeometry( GeometryCache & geometries )
{
	for( auto && component : m_components )
	{
		component.Component()->CollectGeometry( geometries, &m_frame );
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

void Object::SetScene( Scene * scene )
{
	m_scene = scene;
}

Scene * Object::GetScene()
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
