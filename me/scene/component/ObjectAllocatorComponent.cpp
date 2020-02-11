// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/scene/component/ObjectAllocatorComponent.h>
#include <me/scene/GrowableObjectStack.h>

using namespace me;
using namespace scene;
using namespace component;

ObjectAllocatorComponent::ObjectAllocatorComponent( os::IOS * os )
: SceneComponent( os, "ObjectAllocatorComponent" )
{
}

ObjectAllocatorComponent::~ObjectAllocatorComponent()
{
}

void ObjectAllocatorComponent::OnAttach( IScene * scene )
{
	SceneComponent::OnAttach( scene );

	auto stack = new GrowableObjectStack( scene, 2500 );
	m_objectStack.reset( stack );
	AddInterface( "IObjectAllocator", stack );
}

void ObjectAllocatorComponent::OnDetach( IScene * scene ) 
{
	m_objectStack.reset();

	SceneComponent::OnDetach( scene );
}

void ObjectAllocatorComponent::OnUpdate( const UpdateParams & params ) 
{
	m_objectStack->Update( params );
}

void ObjectAllocatorComponent::CollectCameras( RenderGirl & renderGirl )
{	
	m_objectStack->CollectCameras( renderGirl );
}

void ObjectAllocatorComponent::OnRender( RenderGirl & renderGirl ) 
{
	renderGirl.Render( m_objectStack.get() );
}

void ObjectAllocatorComponent::OnSuspend() 
{
	std::vector< object::Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnSuspend();
	}
}

void ObjectAllocatorComponent::OnResume()
{
	std::vector< object::Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnResume();
	}
}

std::string ObjectAllocatorComponent::GetWhat() const
{
	return std::string();
}						 

