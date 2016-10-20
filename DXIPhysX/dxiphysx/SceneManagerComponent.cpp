// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/SceneManagerComponent.h>
#include <dxiphysx/SceneComponent.h>

using namespace dxiphysx;
using namespace dxi;

SceneManagerComponent::SceneManagerComponent( core::IOS * os )
	: m_os( os )
{
}

SceneManagerComponent::~SceneManagerComponent()
{
}

const char * SceneManagerComponent::GetName() const
{	   
	return "PhysXSMC";
}

// TODO: Verify this is necessary.
void DeleterSC( SceneComponent * sc )
{
	delete sc;
}

void SceneManagerComponent::OnNewScene( scene::Scene * scene )
{
	//scene->AddComponent( scene::ISceneComponent::ptr( new SceneComponent( m_os ), DeleterSC ) );
}
