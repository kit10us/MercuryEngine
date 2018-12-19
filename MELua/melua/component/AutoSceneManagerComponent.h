// Copyright (c) 2002 - 2018, Evil Quail LLC
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

			unify::Path GetAutoPath() const;
			void SetAutoPath( unify::Path autoPath );

			unify::Path GetOnceBeforeStartScript() const;
			void SetOnceBeforeStartScript( unify::Path onceBeforeStartScript );

			bool GetOnceBeforeStart() const;

		public: // ISceneComponent... 
			void OnAttach( me::scene::SceneManager * sceneManager ) override;
			void OnDetach( me::scene::SceneManager * sceneManager ) override;
			void OnSceneStart( me::scene::IScene * scene ) override;
			void OnSceneEnd( me::scene::IScene * scene ) override;

		public: // IComponent

		private:

			melua::ScriptEngine * m_scriptEngine;
			unify::Path m_autoPath;
			unify::Path m_onceBeforeStartScript;
			bool m_onceBeforeStart;
		};
	}
}
