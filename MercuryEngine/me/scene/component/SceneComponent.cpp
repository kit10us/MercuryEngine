// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/component/SceneComponent.h>

using namespace me;
using namespace scene;
using namespace component;

SceneComponent::SceneComponent( os::IOS * os, std::string typeName )
	: m_os{ os }
	, m_typeName{ typeName }
	, m_enabled{ true }
	, m_scene{ nullptr }
{
	AddInterface( "IComponent", this );
	AddInterface( "ISceneComponent", this );
	AddInterface( "SceneComponent", this );
	AddInterface( typeName, this );
}

SceneComponent::~SceneComponent()
{
}

os::IOS * SceneComponent::GetOS()
{
	return m_os;
}

const os::IOS * SceneComponent::GetOS() const
{
	return m_os;
}

 IScene * SceneComponent::GetScene()
{
	return m_scene;
}

const IScene * SceneComponent::GetScene() const
{
	return m_scene;
}

void SceneComponent::AddInterface( std::string name, IThing* ptr )
{
	m_interfaceMap[ name ] = ptr;
}

void SceneComponent::OnAttach( IScene * scene )
{
	m_scene = scene;
}

void SceneComponent::OnDetach( IScene * scene ) 
{
	m_scene = nullptr;
}

void SceneComponent::OnBeforeStart() 
{
}

void SceneComponent::OnAfterStart() 
{
}

void SceneComponent::OnEarlyUpdate( const UpdateParams & params )
{
}

void SceneComponent::OnUpdate( const UpdateParams & params ) 
{
}

void SceneComponent::OnLateUpdate( const UpdateParams & params )
{
}

void SceneComponent::CollectCameras( RenderGirl & renderGirl )
{
}

void SceneComponent::OnRender( RenderGirl & renderGirl ) 
{
}

void SceneComponent::OnSuspend() 
{
}

void SceneComponent::OnResume()
{
}

void SceneComponent::OnEnd()
{
}

bool SceneComponent::IsEnabled() const
{
	return m_enabled;
}

void SceneComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

interop::Interop * SceneComponent::GetLookup()
{
	return &m_values;
}

const interop::Interop * SceneComponent::GetLookup() const
{
	return &m_values;
}

me::IThing* SceneComponent::QueryInterface( std::string name )
{
	auto itr = m_interfaceMap.find( name );
	if ( itr == m_interfaceMap.end() )
	{
		return nullptr;
	}

	return itr->second;
}

std::string SceneComponent::GetTypeName() const
{
	return m_typeName;
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}
