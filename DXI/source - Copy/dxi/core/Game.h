// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IGame.h>
#include <dxi/core/IInput.h>
#include <dxi/TextureManager.h>
#include <dxi/EffectManager.h>
#include <dxi/PixelShaderManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/core/IOS.h>
#include <dxi/Exception.h>
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
			
			// Returns Setup: false ends the program immediately.
			bool Initialize( IOS * os );
			
			// Implementations from IGame...

			virtual bool Setup( IOS & os );
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

			virtual IOS & GetOS();
			virtual TextureManager::shared_ptr GetTextureManager();
			virtual EffectManager::shared_ptr GetEffectManager();
			virtual PixelShaderManager::shared_ptr GetPixelShaderManager();
			
			//virtual VertexShaderManager::shared_ptr GetVertexShaderManager();
			template< typename T >
			std::shared_ptr< ResourceManager< T > > GetManager();
			
			virtual GeometryManager::shared_ptr GetGeometryManager();
			virtual scene::SceneManager::shared_ptr GetSceneManager();
			virtual Managers GetManagers();

			template< typename T > std::shared_ptr< T > Find( const std::string & id );

			void RequestQuit();
			bool IsQuitting() const;
			IInput & GetInput();

		private:
			IOS::shared_ptr m_os;
			TextureManager::shared_ptr m_textureManager;
			EffectManager::shared_ptr m_effectManager;
			PixelShaderManager::shared_ptr m_pixelShaderManager;

			VertexShaderManager m_vertexShaderManager;
			//VertexShaderManager::shared_ptr m_vertexShaderManager;

			GeometryManager::shared_ptr m_geometryManager;
			scene::SceneManager::shared_ptr m_sceneManager;

			bool m_isQuitting;
			std::shared_ptr< IInput > m_input;

			static Game * s_gameInstance;

		public:
			static Game * GetGameInstance();
		};

		template<> VertexShaderManager Game::GetManager();

		template<> std::shared_ptr< Texture > Game::Find( const std::string & id );
		template<> std::shared_ptr< Effect > Game::Find( const std::string & id );
		template<> std::shared_ptr< PixelShader > Game::Find( const std::string & id );
		template<> std::shared_ptr< VertexShader > Game::Find( const std::string & id );
		template<> std::shared_ptr< geo::Geometry > Game::Find( const std::string & id );
		template<> std::shared_ptr< scene::Scene > Game::Find( const std::string & id );

	} // namespace core
} // namespace dxi