// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/GeometryCache.h>
#include <me/scene/IObjectAllocator.h>
#include <list>

#ifdef GetObject
#undef GetObject
#endif

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

			size_t Count() const override;
			bool Available() const override;
			
			void SetCache( bool cache );
			bool GetCache() const;
			void ResetCache();

			Object * NewObject( std::string name ) override;
			bool DestroyObject( Object * object ) override;
			Object * CopyObject( Object * from, std::string name ) override; 
			void CollectObjects( std::vector< Object * > & objects ) override;
			Object * FindObject( std::string name ) override;
			Object * GetObject( size_t index ) override;

			void Update( UpdateParams params ) override;
			void CollectCameras( CameraCache & camerasOut ) override;
			void CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & summation ) override;

		private:
			Scene * m_scene;
			std::vector< Object > m_objects;
			int m_nextObjectAvailable;
			int m_lastObjectAlive; 
			int m_freeObjects;
			int m_count;
			bool m_cache;
			bool m_resetCache;

			// All objects enter here.
			std::list< Object * > m_newObjects;

			IObjectComponent::cache m_updatables;
			CameraCache m_cameras;
			GeometryCache m_geometries;
		};
	}
}