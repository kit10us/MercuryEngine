// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxi/scene/IObjectComponent.h>
#include <dxi/scene/Object.h>
#include <dxi/core/IGame.h>

namespace dxilua
{
	class Module : public dxi::scene::IObjectComponent
	{
	protected:
		Module( Module & component );

	public:
		Module( lua_State * state, dxi::core::IGame * game, std::string myName, unify::Path path );
		~Module();

		std::string GetName() const;

		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;

		void OnAttach( dxi::scene::Object * object ) override;
		void OnDetach( dxi::scene::Object * object ) override;
		void OnInit( dxi::scene::Object * object ) override;
		void OnStart( dxi::scene::Object * object ) override;
		void OnUpdate( dxi::scene::Object * object, const dxi::RenderInfo & renderInfo ) override;
		void OnRender( dxi::scene::Object * object, const dxi::RenderInfo & renderInfo ) override;
		void OnSuspend( dxi::scene::Object * object ) override;
		void OnResume( dxi::scene::Object * object ) override;

		dxi::scene::IObjectComponent * Duplicate();

	private: 
		void CallMember( std::string function );

		lua_State * m_state;
		dxi::core::IGame * m_game;
		dxi::scene::Object * m_object;
		std::string m_name;
		unify::Path m_path;
		bool m_enabled;
	};
}
