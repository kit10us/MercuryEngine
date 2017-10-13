// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/Geometry.h>
#include <me/input/InputManager.h>
#include <me/input/IInputCondition.h>
#include <me/IGame.h>
#include <me/IOS.h>
#include <me/render/Display.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>
#include <me/scene/ISceneFactory.h>
#include <rm/ResourceManagerSimple.h>

typedef std::shared_ptr< rm::ISourceFactory< me::render::ITexture > > TextureFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::Effect > > EffectFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::IPixelShader > > PixelShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::IVertexShader > > VertexShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::render::Geometry > > GeometryFactoryPtr;
																	   
namespace me
{
	class Game : public me::IGame
	{
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

		void LogLine( std::string line, int indent = 2 ) override;
		void AttachLogListener( ILogListener* listener ) override;
		void DetachLogListener( ILogListener* litener ) override;

		void ReportError( me::ErrorLevel level, std::string source, std::string error ) override;

		bool HadCriticalError() const override;

		int GetComponentCount() const override;
		void AddComponent( me::IGameComponent::ptr component ) override;
		void RemoveComponent( me::IGameComponent::ptr component ) override;
		me::IGameComponent::ptr GetComponent( int index ) override;
		me::IGameComponent::ptr GetComponent( std::string typeName ) override;
		int FindComponent( std::string typeName ) const override;

		UpdateLock::ptr LockUpdate( bool exclusive );		

		bool IsUpdateLocked( bool exclusive ) const;

		action::IAction::ptr CreateAction(const qxml::Element * element) override;

		object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) override;

		input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) override;

	private:
		void AddExtension( unify::Path path, const qxml::Element * element );

	private:
		std::string m_title;
		scene::ISceneFactory::ptr m_mainSceneFactory;
		OSParameters m_osParameters;
		std::list< me::IGameComponent::ptr > m_components;
			
		bool m_failuresAsCritial;

		unify::Path m_setup;
		unify::Path m_logFile;
		std::list< ILogListener* > m_logListeners;

		float m_totalStartupTime;

		std::shared_ptr< IOS > m_os;
		rm::ResourceHub m_resourceHub;

		render::RenderInfo m_renderInfo;

		bool m_isQuitting;
			
		input::InputManager m_inputManager;
		unify::Owner::ptr m_inputOwnership;

		std::list< std::shared_ptr< Extension > > m_extensions;

		std::list< std::string > m_criticalErrors;

		std::weak_ptr< UpdateLock > m_exclusiveLock;
		std::list< std::weak_ptr< UpdateLock > > m_locks;
	};
}

extern "C" me::Game * GetGameInstance();

#define RegisterGame( GAME )														 \
extern "C" me::Game * GetGameInstance() { return (me::Game*)&GAME; }

