// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/input/InputManager.h>
#include <me/IGame.h>
#include <me/IOS.h>
#include <me/Display.h>
#include <me/ITexture.h>
#include <me/Effect.h>
#include <rm/ResourceManagerSimple.h>

typedef std::shared_ptr< rm::ISourceFactory< me::ITexture > > TextureFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::Effect > > EffectFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::IPixelShader > > PixelShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::IVertexShader > > VertexShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< me::Geometry > > GeometryFactoryPtr;
																	   
namespace me
{
	class Game : public me::IGame
	{
	protected: // User overrides...

		virtual bool Setup( me::IOS * os ) override;
		virtual void Startup() override;		  
		virtual void Update( IRenderer * renderer, RenderInfo & renderInfo ) override;
		virtual void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
		virtual void Shutdown() override;

	public:
		Game( unify::Path setup = "setup.xml" );
		virtual ~Game();

		me::OSParameters GetOSParameters() const override;


		void * Feed( std::string target, void * data ) override;
						
		/// <summary>
		/// Perform necessary initialization.
		/// Returns Setup: false ends the program immediately.
		/// </summary>
		bool Initialize( me::OSParameters osParameters );

		/// <summary>
		/// Optional function to be called on reciept of a drag-and-drop event.
		virtual void OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point );

		// Handles updating...
		virtual void Tick() override;

		// Handles drawing...
		virtual void Draw() override;

		virtual const me::RenderInfo & GetRenderInfo() const final;

		void SetOS( me::IOS::ptr os ) final;
		me::IOS * GetOS() final;

		template< typename T > 
		rm::ResourceManagerSimple< T > * GetManager();

		template<> rm::ResourceManagerSimple< me::ITexture > * GetManager();
		template<> rm::ResourceManagerSimple< me::Effect > * GetManager();
		template<> rm::ResourceManagerSimple< me::IPixelShader > * GetManager();
		template<> rm::ResourceManagerSimple< me::IVertexShader > * GetManager();
		template<> rm::ResourceManagerSimple< me::Geometry > * GetManager();

		rm::ResourceHub & GetResourceHub() override;
		const rm::ResourceHub & GetResourceHub() const override;

		void Quit();
		bool IsQuitting() const;

		me::input::InputManager * GetInputManager();
		const me::input::InputManager * GetInputManager() const;

		void Log( std::string text );
		void LogLine( std::string line );

		void ReportError( me::ErrorLevel level, std::string source, std::string error ) override;

		bool HadCriticalError() const override;

		int ComponentCount() const;
		void AddComponent( me::IGameComponent::ptr component );
		void RemoveComponent( me::IGameComponent::ptr component );
		me::IGameComponent::ptr GetComponent( int index );
		me::IGameComponent::ptr GetComponent( std::string name, int startIndex );
		int FindComponent( std::string name, int startIndex ) const;

		void SetUpdateEnabled( bool enabled ) override;
		bool GetUpdateEnabled() const override;

	private:
		void AddExtension( unify::Path path );

	private:
		std::string m_title;
		me::OSParameters m_osParameters;
		std::list< me::IGameComponent::ptr > m_components;
			
		bool m_failuresAsCritial;

		unify::Path m_setup;
		unify::Path m_logFile;

		me::scene::IObjectComponent::ptr m_gameModule;

		float m_totalStartupTime;

		std::shared_ptr< me::IOS > m_os;
		rm::ResourceHub m_resourceHub;

		me::RenderInfo m_renderInfo;

		bool m_isQuitting;
			
		me::input::InputManager m_inputManager;

		std::list< std::shared_ptr< me::Extension > > m_extensions;

		std::list< std::string > m_criticalErrors;

		bool m_updateEnabled;
	};
}

#define RegisterGame( GAME )														 \
extern "C" me::Game * GetGameInstance() { return (me::Game*)&GAME; }

