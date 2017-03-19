// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <me/scene/SceneManagerComponent.h>

namespace melua
{
	namespace component
	{
		class AutoSceneManagerComponent : public me::scene::SceneManagerComponent
		{
		public:
			AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath );
			~AutoSceneManagerComponent();

		public: // ISceneComponent... 
			void OnAttach( me::scene::SceneManager * sceneManager ) override;
			void OnDetach( me::scene::SceneManager * sceneManager ) override;
			void OnSceneAdded( me::scene::IScene * scene ) override;
			void OnSceneChange( me::scene::IScene * from, me::scene::IScene * to ) override;

		private:
			melua::ScriptEngine * m_scriptEngine;
			unify::Path m_autoPath;
		};
	}
}
