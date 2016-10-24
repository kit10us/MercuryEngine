// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxi/scripting/IModule.h>
#include <dxi/scene/Object.h>
#include <dxi/core/IGame.h>

namespace dxilua
{
	class Module : public dxi::scripting::IModule
	{
	public:
		Module( dxi::scene::Object::ptr object, lua_State * state, dxi::core::IGame * game, std::string myName, unify::Path path );
		~Module();

		void OnInit() override;
		void OnStart() override;
		void OnUpdate() override;
		void OnSuspend() override;
		void OnResume() override;

	private: 
		void CallMember( std::string function );

		lua_State * m_state;
		dxi::core::IGame * m_game;
		dxi::scene::Object::ptr m_object;
		std::string m_name;
		unify::Path m_path;
	};
}
