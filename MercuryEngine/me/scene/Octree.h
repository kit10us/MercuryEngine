// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGame.h>
#include <me/scene/IObjectAllocator.h>
#include <me/object/Object.h>
#include <me/scene/ObjectStack.h>
#include <list>

#ifdef GetObject
#undef GetObject
#endif

namespace me
{
	namespace scene
	{
		class Octree : public IObjectAllocator
		{
		public:
			Octree( IScene * scene, size_t max );
			virtual ~Octree();

			size_t Count() const override;
			bool Available() const override;

			object::Object * NewObject( std::string name ) override;
			bool DestroyObject( object::Object * object ) override;
			object::Object * CopyObject( object::Object * from, std::string name ) override; 
			void CollectObjects( std::vector< object::Object * > & objects ) override;
			object::Object * FindObject( std::string name ) override;
			object::Object* GetObject( size_t index ) override;

			void Update( UpdateParams params ) override;
			void CollectCameras( RenderGirl & renderGirl ) override;
			void CollectRendering( RenderParams params, const object::FinalCamera & camera, GeometryCacheSummation & summation ) override;

		private:
			IScene * m_scene;
			size_t m_max;
			std::list< std::shared_ptr< ObjectStack > > m_stacks;
		};
	}
}