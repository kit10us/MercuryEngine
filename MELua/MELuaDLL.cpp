// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include <MELuaDLL.h>
#include <melua/ScriptEngine.h>
#include <melua/component/GameComponent.h>
#include <melua/component/AutoSceneManagerComponent.h>
#include <memory.h>

void ScriptEngineDeleter( me::IGameComponent * se )
{
	delete se;
}

extern "C" MELUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element);

MELUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element )
{
	// Add Script Engine...
	melua::ScriptEngine * scriptEngine = new melua::ScriptEngine( game->GetOS() );
	game->AddComponent( me::IGameComponent::ptr( scriptEngine, ScriptEngineDeleter ) );

	// Automatically add "game.lua" GameComponent...
	unify::Path source("game.lua");
	if (element->HasElements("startup"))
	{
		source = element->GetElement("startup")->GetText();
		unify::Path(game->GetOS()->GetAssetPaths().FindAsset(source));
	}
	source = unify::Path(game->GetOS()->GetAssetPaths().FindAsset(source));

	// Auto-scene script scene manager component..
	unify::Path autoPath;
	if (element->HasElements("auto"))
	{
		unify::Path autoPath(element->GetElement("auto")->GetText());
	}
	auto autoSceneManagerComponent = new melua::component::AutoSceneManagerComponent( scriptEngine, autoPath + "scene/", source );
	auto sceneManager = game->GetComponentT< me::scene::SceneManager >( "SceneManager" );
	sceneManager->AddComponent( me::scene::ISceneManagerComponent::ptr( autoSceneManagerComponent ) );		  

	return true;
}

