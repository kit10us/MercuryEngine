// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/ObjectAllocator.h>
#include <me/object/Object.h>
#include <me/scene/ObjectStack.h>
#include <me/IUnknown.h>
#include <list>

namespace me
{
	namespace scene
	{
		class Scene;

		class GrowableObjectStack : public ObjectAllocator
		{
		public:
			GrowableObjectStack( Scene * scene, size_t max );
			virtual ~GrowableObjectStack();

			size_t Count() const override;
			bool Available() const override;

			object::Object * NewObject( std::string name ) override;
			bool DestroyObject( object::Object * object ) override;
			object::Object * CopyObject( object::Object * from, std::string name ) override; 
			void CollectObjects( std::vector< object::Object * > & objects ) override;
			object::Object * FindObject( std::string name ) override;
			object::Object * GetObject( size_t index ) override;

			void Update( UpdateParams params ) override;
			void CollectCameras( object::CameraCache & camerasOut ) override;
			void CollectRendering( RenderParams params, const object::FinalCamera & camera, GeometryCacheSummation & summation ) override;

		private:
			Scene * m_scene;
			size_t m_max;
			std::list< std::shared_ptr< ObjectStack > > m_stacks;
		};
	}
}