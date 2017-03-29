// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/GeometryCache.h>
#include <me/scene/RenderGirl.h>
#include <me/scene/ObjectAllocator.h>
#include <me/scene/IScene.h>
#include <me/object/Object.h>
#include <list>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	namespace scene
	{
		class ObjectStack : public ObjectAllocator
		{
		public:
			ObjectStack( IScene * scene, size_t max );
			virtual ~ObjectStack();

			size_t Count() const override;
			bool Available() const override;
			
			void SetCache( bool cache );
			bool GetCache() const;
			void ResetCache();

			object::Object * NewObject( std::string name ) override;
			bool DestroyObject( object::Object * object ) override;
			object::Object * CopyObject( object::Object * from, std::string name ) override; 
			void CollectObjects( std::vector< object::Object * > & objects ) override;
			object::Object * FindObject( std::string name ) override;
			object::Object * GetObject( size_t index ) override;
			void DirtyObject( object::Object* object ) override;

			void Update( UpdateParams params ) override;
			void CollectCameras( RenderGirl & renderGirl ) override;
			void CollectRendering( RenderParams params, const object::FinalCamera & camera, GeometryCacheSummation & solids, GeometryCacheSummation & trans ) override;

		private:
			IScene * m_scene;
			std::vector< object::Object > m_objects;
			int m_nextObjectAvailable;
			int m_lastObjectAlive; 
			int m_freeObjects;
			int m_count;
			bool m_cache;
			bool m_resetCache;

			// All objects enter here.
			std::list< object::Object * > m_newObjects;

			object::IObjectComponent::cache m_updatables;
			object::CameraCache m_cameras;
			GeometryCache m_solids;
			GeometryCache m_trans;
		};
	}
}