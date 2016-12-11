																																																					 // Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/scene/IObjectAllocator.h>
#include <me/scene/Object.h>
#include <list>

namespace me
{
	namespace scene
	{
		class Scene;

		class ObjectStack : public IObjectAllocator
		{
		public:
			ObjectStack( Scene * scene, size_t max );
			virtual ~ObjectStack();

			Object * NewObject( std::string name ) override;
			bool DestroyObject( Object * object ) override;
			Object * CopyObject( Object * from, std::string name ) override; 
			void CollectObjects( std::vector< Object * > & objects ) override;
			Object * FindObject( std::string name );

			void Update( IRenderer * renderer, const RenderInfo & renderInfo ) override;
			void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;

		private:
			Scene * m_scene;
			std::vector< Object > m_objects;
			size_t m_nextObjectAvailable;
			size_t m_lastObjectAlive;


			struct FinalCamera
			{
				Object * object;
				class CameraComponent * camera;
			};
			std::list< FinalCamera > m_cached_cameraList;
			bool m_needRenderCaching;
			std::map< Geometry *, std::vector< const unify::FrameLite * > > m_cached_sorted;

			bool m_needUpdateCaching;
			std::vector< Object * > m_cached_updates;
		};
	}
}