// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/object/component/BBoxRendererComponent.h>
#include <me/object/Object.h>

using namespace me;
using namespace scene;
using namespace object;
using namespace render;

AutoBBoxSceneComponent::AutoBBoxSceneComponent( os::IOS * os, Effect::ptr effect )
	: SceneComponent( os, "AutoBBox" )
	, m_effect( effect )
	, m_padding( 0.1f )
	, m_color( unify::ColorBlue( 155 ) )
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

void AutoBBoxSceneComponent::OnUpdate( const UpdateParams & params ) 
{
	// Ensure all objects have BBoxes...
	std::vector< Object * > objects;
	GetScene()->GetObjectAllocator()->CollectObjects( objects );
	for( auto && object : objects )
	{
		if ( object->GetComponent( "BBoxRenderer" ) ) continue;
		if ( ! object->GetComponent( "Geometry" ) ) continue;
		
		object->AddComponent( component::IObjectComponent::ptr( new component::BBoxRendererComponent( GetOS(), m_effect, m_color ) ) );
	}
}

std::string AutoBBoxSceneComponent::GetWhat() const
{
	return std::string();
}
