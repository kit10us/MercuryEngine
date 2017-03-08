// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/SceneManagerComponent.h>
#include <mephysx/SceneComponent.h>

using namespace mephysx;
using namespace me;
using namespace scene;

SceneManagerComponent::SceneManagerComponent( IOS * os )
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

void SceneManagerComponent::OnNewScene( Scene * scene )
{
	//scene->AddComponent( scene::ISceneComponent::ptr( new SceneComponent( m_os ), DeleterSC ) );
}
