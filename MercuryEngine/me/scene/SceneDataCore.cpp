// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/SceneDataCore.h>

using namespace me;
using namespace scene;

SceneDataCore::SceneDataCore()
{
}

SceneDataCore::~SceneDataCore()
{
}

bool SceneDataCore::IsEnabled() const
{
	return true;
}

void SceneDataCore::SetEnabled( bool enabled )
{
}

void SceneDataCore::OnAttach( Scene * scene )
{
}

void SceneDataCore::OnDetach( Scene * scene )
{
}

void SceneDataCore::OnInit( Scene * scene )
{
}

void SceneDataCore::OnStart( Scene * scene )
{
}

void SceneDataCore::OnUpdate( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo )
{
}

void SceneDataCore::OnRender( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo )
{
}

void SceneDataCore::OnSuspend()
{
}

void SceneDataCore::OnResume()
{
}
