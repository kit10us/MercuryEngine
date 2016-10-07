// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IGame.h>
#include <dxi/core/IInput.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/core/IOS.h>
#include <dxi/exception/Exception.h>
#include <rm/ResourceManagerSimple.h>
#include <rm/ResourceHub.h>

typedef std::shared_ptr< rm::ISourceFactory< dxi::Texture > > TextureFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< dxi::Effect > > EffectFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< dxi::PixelShader > > PixelShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< dxi::VertexShader > > VertexShaderFactoryPtr;
typedef std::shared_ptr< rm::ISourceFactory< dxi::Geometry > > GeometryFactoryPtr;
																	   
namespace dxi
{
	namespace core
	{
		class Game : public IGame
		{
		protected: // User overrides...

			virtual bool Setup( IOS & os ) override;  
			virtual void Startup() override;		  
			virtual bool Update( RenderInfo & renderInfo, IInput & input ) override;
			virtual void Render( const RenderInfo & renderInfo ) override;			

		public:
			Game( unify::Path setup = "setup.xml" );
			virtual ~Game();
						
			/// <summary>
			/// Perform necessary initialization.
			/// Returns Setup: false ends the program immediately.
			/// </summary>
			bool Initialize( std::shared_ptr< IOS > os );

			/// <summary>
			/// Optional function to be called on reciept of a drag-and-drop event.
			virtual void OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point );

			// Handles updating...
			virtual void Tick() override;

			// Handles drawing...
			virtual void Draw() override;

			virtual void Shutdown() override;

			virtual IOS & GetOS() final;

			virtual const RenderInfo & GetRenderInfo() const final;

			void AddScriptEngine( std::string name, std::shared_ptr< scripting::IScriptEngine > se ) override;

			scripting::IScriptEngine * GetScriptEngine( std::string name ) override;

			virtual scene::SceneManager::shared_ptr GetSceneManager();

			scene::Scene::shared_ptr FindScene( const std::string & id );

			template< typename T > 
			rm::ResourceManagerSimple< T > * GetManager();

			template<> rm::ResourceManagerSimple< Texture > * GetManager();
			template<> rm::ResourceManagerSimple< Effect > * GetManager();
			template<> rm::ResourceManagerSimple< PixelShader > * GetManager();
			template<> rm::ResourceManagerSimple< VertexShader > * GetManager();
			template<> rm::ResourceManagerSimple< Geometry > * GetManager();

			rm::ResourceHub & GetResourceHub();
			const rm::ResourceHub & GetResourceHub() const;

			void RequestQuit();
			bool IsQuitting() const;
			IInput & GetInput();

			void AddExtension( std::shared_ptr< Extension > extension ) override;

			void Log( std::string text );
			void LogLine( std::string line );

		private:
			void BeforeUpdate();
			void AfterUpdate();

			void BeforeRender();
			void AfterRender();	

			unify::Path m_setup;
			unify::Path m_logFile;
			scripting::IModule::ptr m_gameModule;

			std::shared_ptr< IOS > m_os;
			rm::ResourceHub m_resourceHub;

			RenderInfo m_renderInfo;
			
			scene::SceneManager::shared_ptr m_sceneManager;

			bool m_isQuitting;
			std::shared_ptr< IInput > m_input;

			std::map < std::string, std::shared_ptr< scripting::IScriptEngine >, unify::CaseInsensitiveLessThanTest > m_scriptEngines;

			std::list< std::shared_ptr< Extension > > m_extensions;

			static Game * s_gameInstance;

		public:
			static Game * GetInstance();
		};
	}
}