// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <me/IObjectComponent.h>
#include <me/Object.h>
#include <me/IGame.h>

namespace dxilua
{
	class Module : public me::IObjectComponent
	{
	protected:
		Module( Module & component );

	public:
		Module( lua_State * state, me::IGame * game, std::string myName, unify::Path path );
		~Module();

		std::string GetName() const;

		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;

		void OnAttach( me::Object * object ) override;
		void OnDetach( me::Object * object ) override;
		void OnInit( me::Object * object ) override;
		void OnStart( me::Object * object ) override;
		void OnUpdate( me::Object * object, const me::RenderInfo & renderInfo ) override;
		void OnRender( me::Object * object, const me::RenderInfo & renderInfo ) override;
		void OnSuspend( me::Object * object ) override;
		void OnResume( me::Object * object ) override;

		me::IObjectComponent * Duplicate();

	private: 
		void CallMember( std::string function );

		lua_State * m_state;
		me::IGame * m_game;
		me::Object * m_object;
		std::string m_name;
		unify::Path m_path;
		bool m_enabled;
	};
}
