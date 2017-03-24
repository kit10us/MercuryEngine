// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/ObjectAllocatorComponent.h>
#include <me/scene/GrowableObjectStack.h>

using namespace me;
using namespace scene;
using namespace object;

ObjectAllocatorComponent::ObjectAllocatorComponent( IOS * os )
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

void ObjectAllocatorComponent::OnUpdate( UpdateParams params ) 
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
	std::vector< Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnSuspend();
	}
}

void ObjectAllocatorComponent::OnResume()
{
	std::vector< Object * > objects;
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
