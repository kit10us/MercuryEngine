// Copyright (c) 2002 - 2018, Quentin S. Smith
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
			AutoSceneManagerComponent( melua::ScriptEngine * scriptEngine, unify::Path autoPath, unify::Path onceBeforeStart );
			~AutoSceneManagerComponent();

		public: // ISceneComponent... 
			void OnAttach( me::scene::SceneManager * sceneManager ) override;
			void OnDetach( me::scene::SceneManager * sceneManager ) override;
			void OnSceneStart( me::scene::IScene * scene ) override;
			void OnSceneEnd( me::scene::IScene * scene ) override;

		private:
			melua::ScriptEngine * m_scriptEngine;
			unify::Path m_autoPath;
			unify::Path m_onceBeforeStart;
			bool m_once;
		};
	}
}
