// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/game/IGame.h>
#include <me/input/IInputCondition.h>
#include <me/render/Geometry.h>
#include <me/render/Display.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>

typedef std::shared_ptr< rm::ISourceFactory< me::render::ITexture > > TextureFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::Effect > > EffectFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::IPixelShader > > PixelShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::IVertexShader > > VertexShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::Geometry > > GeometryFactoryPtr;
																	   
namespace me
{
	namespace game
	{
		class Game : public IGame
		{
			struct CommandListenerSet
			{
				unify::Owner::weak_ptr owner;
				ICommandListener::ptr listener;
			};

		protected: // User overrides...

			virtual bool Setup( IOS * os ) override;
			virtual void Startup() override;
			virtual void Shutdown() override;

		public:
			Game( unify::Path setup = unify::Path( "setup.xml" ) );
			Game( scene::ISceneFactory::ptr mainSceneFactory, unify::Path setup = unify::Path( "setup.xml" ) );
			virtual ~Game();

			OSParameters GetOSParameters() const override;

			void * Feed( std::string target, void * data ) override;

			/// <summary>
			/// Perform necessary initialization.
			/// Returns Setup: false ends the program immediately.
			/// </summary>
			void Initialize( me::OSParameters osParameters );

			/// <summary>
			/// Optional function to be called on reciept of a drag-and-drop event.
			virtual void OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point );

			// Handles updating...
			virtual void Tick() override;

			// Handles drawing...
			virtual void Draw() override;

			virtual const render::RenderInfo & GetRenderInfo() const final;

			void SetOS( IOS::ptr os ) final;
			IOS * GetOS() final;
			const IOS * GetOS() const final;

			template< typename T >
			rm::ResourceManagerSimple< T > * GetManager();

			template<> rm::ResourceManagerSimple< render::ITexture > * GetManager();
			template<> rm::ResourceManagerSimple< render::Effect > * GetManager();
			template<> rm::ResourceManagerSimple< render::IPixelShader > * GetManager();
			template<> rm::ResourceManagerSimple< render::IVertexShader > * GetManager();
			template<> rm::ResourceManagerSimple< render::Geometry > * GetManager();

			rm::ResourceHub & GetResourceHub() override;
			const rm::ResourceHub & GetResourceHub() const override;

			void Quit();
			bool IsQuitting() const;

			unify::Owner::ptr GetOwnership();

			me::input::InputManager * GetInputManager();
			const me::input::InputManager * GetInputManager() const;

			int GetComponentCount() const override;
			void AddComponent( game::IGameComponent::ptr component ) override;
			void RemoveComponent( game::IGameComponent::ptr component ) override;
			game::IGameComponent::ptr GetComponent( int index ) override;
			game::IGameComponent::ptr GetComponent( std::string typeName ) override;
			int FindComponent( std::string typeName ) const override;

			UpdateLock::ptr LockUpdate( bool exclusive );

			bool IsUpdateLocked( bool exclusive ) const;

			action::IAction::ptr CreateAction( const qxml::Element * element ) override;

			object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) override;

			input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) override;

			void AddCommandListener( unify::Owner::weak_ptr owner, std::string command, ICommandListener::ptr listener ) override;

			size_t Command( std::string command ) override;

			std::string Command( size_t id ) override;

			std::string SendCommand( std::string command, std::string extra ) override;

			std::string SendCommand( size_t id, std::string extra ) override;

			me::os::IDebug * Debug() override;
			const me::os::IDebug * Debug() const override;

		protected:
			void Private_Shutdown();

		private:
			void AddExtension( unify::Path path, const qxml::Element * element );

		private:
			std::string m_title;
			scene::ISceneFactory::ptr m_mainSceneFactory;
			OSParameters m_osParameters;
			std::list< me::game::IGameComponent::ptr > m_components;
			
			unify::Path m_setup;

			float m_totalStartupTime;

			std::shared_ptr< IOS > m_os;
			rm::ResourceHub m_resourceHub;

			render::RenderInfo m_renderInfo;

			bool m_isQuitting;

			input::InputManager m_inputManager;
			unify::Owner::ptr m_inputOwnership;

			std::list< std::shared_ptr< Extension > > m_extensions;

			std::weak_ptr< UpdateLock > m_exclusiveLock;
			std::list< std::weak_ptr< UpdateLock > > m_locks;

			std::vector< std::list< CommandListenerSet > > m_commandListeners;
			std::map< std::string /* command */, size_t /* command ID */, unify::CaseInsensitiveLessThanTest> m_commandMap;
			std::vector< std::string > m_commandList;
		};
	}
}

extern "C" me::game::Game * GetGameInstance();

#define RegisterGame( GAME )														 \
extern "C" me::game::Game * GetGameInstance() { return (me::game::Game*)&GAME; }

