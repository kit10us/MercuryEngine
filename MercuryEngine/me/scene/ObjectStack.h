// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/scene/IObjectAllocator.h>
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

			bool IsResizable() const override;
			size_t Count() const override;
			bool Available() const override;

			Object * NewObject( std::string name ) override;
			bool DestroyObject( Object * object ) override;
			Object * CopyObject( Object * from, std::string name ) override; 
			void CollectObjects( std::vector< Object * > & objects ) override;
			Object * FindObject( std::string name );

			void Update( IRenderer * renderer, const RenderInfo & renderInfo, CameraCache & cameras ) override;
			void ObjectStack::CollectRendering( IRenderer * renderer, const RenderInfo & renderInfo, GeometryCacheSummation & summation ) override;

		private:
			Scene * m_scene;
			std::vector< Object > m_objects;
			size_t m_nextObjectAvailable;
			size_t m_lastObjectAlive; 
			size_t m_freeObjects;
			size_t m_count;

			// All objects enter here.
			std::list< Object * > m_newObjects;

			IObjectComponent::cache m_updatables;
			CameraCache m_cameras;
			GeometryCache m_geometries;
		};
	}
}