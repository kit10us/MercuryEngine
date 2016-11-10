// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <me/scene/IObjectComponent.h>
#include <me/scene/Object.h>
#include <me/IGame.h>

namespace dxilua
{
	class Module : public me::scene::IObjectComponent
	{
	protected:
		Module( Module & component );

	public:
		Module( lua_State * state, me::IGame * game, std::string myName, unify::Path path );
		~Module();

		std::string GetName() const;

		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;

		void OnAttach( me::scene::Object * object ) override;
		void OnDetach( me::scene::Object * object ) override;
		void OnInit( me::scene::Object * object ) override;
		void OnStart( me::scene::Object * object ) override;
		void OnUpdate( me::scene::Object * object, const me::RenderInfo & renderInfo ) override;
		void OnRender( me::scene::Object * object, const me::RenderInfo & renderInfo ) override;
		void OnSuspend( me::scene::Object * object ) override;
		void OnResume( me::scene::Object * object ) override;

		me::scene::IObjectComponent * Duplicate();

	private: 
		void CallMember( std::string function );

		lua_State * m_state;
		me::IGame * m_game;
		me::scene::Object * m_object;
		std::string m_name;
		unify::Path m_path;
		bool m_enabled;
	};
}
