// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/BBoxRendererComponent.h>
#include <me/object/Object.h>

using namespace me;
using namespace scene;
using namespace object;

AutoBBoxSceneComponent::AutoBBoxSceneComponent( IOS * os, Effect::ptr effect )
	: SceneComponent( os, "AutoBBox" )
	, m_effect( effect )
	, m_padding( 0.1f )
	, m_color( unify::Color::ColorBlue( 155 ) )
{
}

AutoBBoxSceneComponent::~AutoBBoxSceneComponent()
{
}

float AutoBBoxSceneComponent::GetPadding() const
{
	return m_padding;
}

void AutoBBoxSceneComponent::SetPadding( float padding )
{
	m_padding = padding;
}

void AutoBBoxSceneComponent::OnUpdate( Scene * scene, UpdateParams params )
{
	// Ensure all objects have BBoxes...
	std::vector< Object * > objects;
	scene->CollectObjects( objects );
	for( auto && object : objects )
	{
		if ( object->GetComponent( "BBoxRenderer", 0 ) ) continue;
		if ( ! object->GetComponent( "Geometry", 0 ) ) continue;
		
		object->AddComponent( IObjectComponent::ptr( new BBoxRendererComponent( GetOS(), m_effect, m_color ) ) );
	}
}

std::string AutoBBoxSceneComponent::GetWhat() const
{
	return std::string();
}
