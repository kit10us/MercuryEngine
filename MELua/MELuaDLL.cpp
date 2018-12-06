// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include <MELuaDLL.h>
#include <melua/ScriptEngine.h>
#include <melua/component/GameComponent.h>
#include <melua/component/AutoSceneManagerComponent.h>
#include <memory.h>

void ScriptEngineDeleter( me::game::IGameComponent * se )
{
	delete se;
}

extern "C" MELUADLL_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element);

MELUADLL_API bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	// Add Script Engine...
	melua::ScriptEngine * scriptEngine = new melua::ScriptEngine( gameInstance );
	gameInstance->AddComponent( me::game::IGameComponent::ptr( scriptEngine, ScriptEngineDeleter ) );

	// Automatically add "game.lua" GameComponent...
	unify::Path startup("game.lua");
	if (element->HasElements("startup"))
	{
		startup = unify::Path( element->GetElement("startup")->GetText() );
		gameInstance->Debug()->LogLine( "Lau::MELoader", "\"startup\" node found, setting startupOnce to \"" + startup.ToString() + "\"." );
	}
	else
	{
		gameInstance->Debug()->LogLine( "Lau::MELoader", "\"startup\" node NOT found." );
	}

	try
	{
		startup = unify::Path( gameInstance->GetOS()->GetAssetPaths().FindAsset( startup ) );
	}
	catch( ... )
	{
		startup = unify::Path();
	}

	// Auto-scene script scene manager component..
	unify::Path autoPath;
	if (element->HasElements("auto"))
	{
		autoPath = unify::Path( element->GetElement("auto")->GetText() );
		gameInstance->Debug()->LogLine( "Lau::MELoader", "\"auto\" node found, setting autoPath to \"" + autoPath.ToString() + "\"." );
	}
	else
	{
		gameInstance->Debug()->LogLine( "Lau::MELoader", "\"auto\" node NOT found." );
	}

	auto autoSceneManagerComponent = new melua::component::AutoSceneManagerComponent( scriptEngine, autoPath + unify::Path( "scene/" ), startup);
	auto sceneManager = gameInstance->GetComponentT< me::scene::SceneManager >();
	sceneManager->AddComponent( me::scene::ISceneManagerComponent::ptr( autoSceneManagerComponent ) );		  

	return true;
}

