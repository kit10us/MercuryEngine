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

unify::Result<> ObjectAllocatorComponent::OnAttach( IScene * scene )
{
	SceneComponent::OnAttach( scene );

	auto stack = new GrowableObjectStack( scene, 2500 );
	m_objectStack.reset( stack );
	AddInterface( "IObjectAllocator", stack );

	return {};
}

unify::Result<> ObjectAllocatorComponent::OnDetach( IScene * scene ) 
{
	m_objectStack.reset();

	auto result = SceneComponent::OnDetach( scene );
	if (!result)
	{
		return result;
	}

	return {};
}

unify::Result<> ObjectAllocatorComponent::OnUpdate( const UpdateParams & params ) 
{
	m_objectStack->Update( params );
	return {};
}

void ObjectAllocatorComponent::CollectCameras( RenderGirl & renderGirl )
{	
	m_objectStack->CollectCameras( renderGirl );
}

unify::Result<> ObjectAllocatorComponent::OnRender( RenderGirl & renderGirl ) 
{
	renderGirl.Render( m_objectStack.get() );
	return {};
}

unify::Result<> ObjectAllocatorComponent::OnSuspend() 
{
	std::vector< object::Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnSuspend();
	}
	return {};
}

unify::Result<> ObjectAllocatorComponent::OnResume()
{
	std::vector< object::Object * > objects;
	m_objectStack->CollectObjects( objects );
	for( auto && object : objects )
	{
		object->OnResume();
	}
	return {};
}

std::string ObjectAllocatorComponent::GetWhat() const
{
	return std::string();
}						 

