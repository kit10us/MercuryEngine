// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/scene/IObjectAllocator.h>
#include <me/scene/Object.h>
#include <me/scene/ObjectStack.h>
#include <list>

namespace me
{
	namespace scene
	{
		class Scene;

		class BasicObjectAllocatorSceneComponent : public IObjectAllocator
		{
		public:
			BasicObjectAllocatorSceneComponent( Scene * scene, size_t max );
			virtual ~BasicObjectAllocatorSceneComponent();

			size_t Count() const override;
			bool Available() const override;

			Object * NewObject( std::string name ) override;
			bool DestroyObject( Object * object ) override;
			Object * CopyObject( Object * from, std::string name ) override; 
			void CollectObjects( std::vector< Object * > & objects ) override;
			Object * FindObject( std::string name ) override;
			Object* GetObject( size_t index ) override;

			void Update( UpdateParams params ) override;
			void CollectCameras( CameraCache & camerasOut ) override;
			void CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & summation ) override;

		private:
			Scene * m_scene;
			size_t m_max;
			std::list< std::shared_ptr< ObjectStack > > m_stacks;
		};
	}
}