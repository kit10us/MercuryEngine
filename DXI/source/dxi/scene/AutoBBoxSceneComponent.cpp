// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/AutoBBoxSceneComponent.h>
#include <dxi/scene/BBoxRendererComponent.h>
#include <dxi/scene/Object.h>

using namespace dxi;
using namespace scene;

AutoBBoxSceneComponent::AutoBBoxSceneComponent( core::IOS * os, Effect::ptr effect )
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

void AutoBBoxSceneComponent::OnUpdate( Scene * scene, const RenderInfo & renderInfo )
{
	// Ensure all objects have BBoxes...
	for( auto object : scene->GetRoot()->AllChildren( true ) )
	{
		if ( object->GetComponent( "BBoxRenderer", 0 ) ) continue;
		if ( ! object->GetComponent( "Geometry", 0 ) ) continue;
		
		object->AddComponent( scene::IObjectComponent::ptr( new BBoxRendererComponent( GetOS(), m_effect, m_color ) ) );
	}
}
