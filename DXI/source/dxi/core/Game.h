// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IGame.h>
#include <dxi/core/IInput.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/core/IOS.h>
#include <dxi/exception/Exception.h>
#include <rm/ResourceManagerSimple.h>
#include <unify/TimeDelta.h>

namespace dxi
{
	namespace core
	{
		class Game : public IGame
		{
		public:
			Game();
			virtual ~Game();
			
			/// <summary>
			/// Returns Setup: false ends the program immediately.
			/// </summary>
			bool Initialize( IOS * os );
			
			/// <summary>
			/// Setup is the initial event that is called to setup the game. It is the earliest point to instigate configuration.
			/// </summary>
			virtual bool Setup( IOS * os );

			virtual void Startup();

			/// <summary>
			/// Optional function to be called on reciept of a drag-and-drop event.
			virtual void OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point );

			virtual void BeforeUpdate();
			virtual bool Update( unify::Seconds elapsed, IInput & input );
			virtual void AfterUpdate();

			virtual void BeforeRender();
			virtual void Render();
			virtual void AfterRender();

			virtual void Shutdown();

			virtual IOS * GetOS();
			
			virtual scene::SceneManager::shared_ptr GetSceneManager();

			scene::Scene::shared_ptr FindScene( const std::string & id );

			template< typename T > 
			std::shared_ptr< rm::ResourceManagerSimple< T > > GetManager();

			void RequestQuit();
			bool IsQuitting() const;
			IInput & GetInput();

		private:
			IOS::shared_ptr m_os;
			std::shared_ptr< rm::ResourceManagerSimple< Texture > > m_textureManager;
			std::shared_ptr< rm::ResourceManagerSimple< Effect > > m_effectManager;
			std::shared_ptr< rm::ResourceManagerSimple< PixelShader > > m_pixelShaderManager;
			std::shared_ptr< rm::ResourceManagerSimple< VertexShader > > m_vertexShaderManager;
			std::shared_ptr< rm::ResourceManagerSimple< Geometry > > m_geometryManager;
			scene::SceneManager::shared_ptr m_sceneManager;

			bool m_isQuitting;
			std::shared_ptr< IInput > m_input;

			static Game * s_gameInstance;

		public:
			static Game * GetGameInstance();
		};
	}
}