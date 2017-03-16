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

void ObjectAllocatorComponent::OnAttach( Scene * scene )
{
	auto stack = new GrowableObjectStack( scene, 2500 );
	m_objectStack.reset( stack );
	AddInterface( "IObjectAllocator", stack );
}

void ObjectAllocatorComponent::OnDetach( Scene * scene ) 
{
	m_objectStack.reset();
}

void ObjectAllocatorComponent::OnInit( Scene * scene ) 
{
}

void ObjectAllocatorComponent::OnStart( Scene * scene ) 
{
}

void ObjectAllocatorComponent::OnUpdate( Scene * scene, UpdateParams params ) 
{
	m_objectStack->Update( params );
}

void ObjectAllocatorComponent::CollectCameras( RenderGirl & renderGirl )
{	
	m_objectStack->CollectCameras( renderGirl );
}

void ObjectAllocatorComponent::OnRender( Scene * scene, RenderGirl & renderGirl ) 
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

