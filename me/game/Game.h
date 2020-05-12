// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/game/IGame.h>
#include <me/render/Geometry.h>
#include <me/render/Display.h>
#include <me/script/IScript.h>
#include <me/render/ITexture.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>
#include <me/script/IScript.h>
#include <me/debug/block.h>
#include <unify/String.h>

namespace me::game
{
	/// <summary>
	/// Standard game implementation.
	/// </summary>
	class Game : public IGame
	{
		struct CommandListenerSet
		{
			unify::Owner::weak_ptr owner;
			ICommandListener::ptr listener;
		};

		std::string m_title;
		os::IOS::ptr m_os;
		std::string m_startScene;
		unify::Path m_setup;
		bool m_isQuitting;
		float m_totalStartupTime;
		unify::Owner::ptr m_inputOwnership;
		input::InputManager m_inputManager;

		std::list< me::game::component::IGameComponent::ptr > m_components;
		rm::ResourceHub m_resourceHub;
		render::RenderInfo m_renderInfo;

		std::list< os::IExtension::ptr > m_extensions;
		std::weak_ptr< UpdateLock > m_exclusiveLock;
		std::list< std::weak_ptr< UpdateLock > > m_locks;

		std::vector< std::list< CommandListenerSet > > m_commandListeners;
		std::map< std::string /* command */, size_t /* command ID */, unify::string::CaseInsensitiveLessThanTest> m_commandMap;
		std::vector< std::string > m_commandList;

		std::shared_ptr< kit::debug::IBlock > m_gameBlock;

	protected: // User overrides...
		virtual bool Setup( os::IOS* os ) override;
		virtual void AddScenes( scene::SceneManager* sceneManager ) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

	public:
		Game( unify::Path setup );
		Game( std::string startScene, unify::Path setup );
		virtual ~Game();

		/// <summary>
		/// Optional function to be called on reciept of a drag-and-drop event.
		/// </summary>
		virtual void OnDragDrop( const std::vector< unify::Path >& files, const unify::V2< float >& point );

	public: // me::game::IGame
		void* Feed( std::string target, void* data ) override;
		void Initialize( os::IOS::ptr os ) override;
		void Tick() override;
		void Draw() override;

		virtual const render::RenderInfo& GetRenderInfo() const final;

		void SetOS( os::IOS::ptr os ) final;
		os::IOS* GetOS() final;
		const os::IOS* GetOS() const final;

		template< typename T >
		rm::ResourceManager< T >* GetManager();

		template<> rm::ResourceManager< script::IScript >* GetManager();
		template<> rm::ResourceManager< render::ITexture >* GetManager();
		template<> rm::ResourceManager< render::Effect >* GetManager();
		template<> rm::ResourceManager< render::IPixelShader >* GetManager();
		template<> rm::ResourceManager< render::IVertexShader >* GetManager();
		template<> rm::ResourceManager< render::Geometry >* GetManager();

		rm::ResourceHub& GetResourceHub() override;
		const rm::ResourceHub& GetResourceHub() const override;

		void Quit();
		bool IsQuitting() const;

		unify::Owner::ptr GetOwnership();

		me::input::InputManager* GetInputManager();
		const me::input::InputManager* GetInputManager() const;

		int GetComponentCount() const override;
		void AddComponent( game::component::IGameComponent::ptr component ) override;
		void RemoveComponent( game::component::IGameComponent::ptr component ) override;
		game::component::IGameComponent::ptr GetComponent( int index ) override;
		game::component::IGameComponent::ptr GetComponent( std::string typeName ) override;
		int FindComponent( std::string typeName ) const override;

		UpdateLock::ptr LockUpdate( bool exclusive );

		bool IsUpdateLocked( bool exclusive ) const;

		action::IAction::ptr CreateAction( const qxml::Element* element ) override;

		object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element* element ) override;

		input::IInputAction::ptr CreateInputAction( const qxml::Element* element ) override;

		void AddCommandListener( unify::Owner::weak_ptr owner, std::string command, ICommandListener::ptr listener ) override;

		size_t Command( std::string command ) override;

		std::string Command( size_t id ) override;

		std::string SendCommand( std::string command, std::string extra ) override;

		std::string SendCommand( size_t id, std::string extra ) override;

		me::debug::IDebug* Debug() override;
		const me::debug::IDebug* Debug() const override;

	protected:
		/// <summary>
		/// Executed upon the shutdown of our game.
		/// </summary>
		void Private_Shutdown();

	private:
		/// <summary>
		/// Add a game extension.
		/// Game extensions extend the features of various game components and events.
		/// </summary>
		void AddExtension( unify::Path path, const qxml::Element* element );
	};
}

extern "C" me::game::Game * GetGameInstance();

#define RegisterGame( GAME )														 \
extern "C" me::game::Game * GetGameInstance() { return (me::game::Game*)&GAME; }

