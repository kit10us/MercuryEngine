// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/ScriptComponent.h>

using namespace dxi;
using namespace scene;

ScriptComponent::ScriptComponent( core::IOS * os )
: Component( os, "Script" )
{
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::SetModule( scripting::IModule::ptr module )
{
	m_module = module;
}

scripting::IModule::ptr ScriptComponent::GetModule()
{
	return m_module;
}

void ScriptComponent::OnInit( Object * object )
{
	m_module->OnInit();
}

void ScriptComponent::OnStart( Object * object )
{
	m_module->OnStart();
}

void ScriptComponent::Update( const RenderInfo & renderInfo )
{
	m_module->OnUpdate();
}

void ScriptComponent::Render( const RenderInfo & renderInfo )
{
}

void ScriptComponent::OnSuspend()
{
}

void ScriptComponent::OnResune()
{
}
