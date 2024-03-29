// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/game/IGame.h>
#include <me/scene/IObjectAllocator.h>
#include <me/object/Object.h>
#include <me/scene/ObjectStack.h>
#include <list>

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

			void Update( const UpdateParams & params ) override;
			void CollectCameras( RenderGirl & renderGirl ) override;
			void CollectRendering( render::Params params, const object::FinalCamera & camera, render::GeometryCacheSummation & solids, render::GeometryCacheSummation & trans ) override;

		private:
			IScene * m_scene;
			size_t m_max;
			std::list< std::shared_ptr< ObjectStack > > m_stacks;
		};
	}
}