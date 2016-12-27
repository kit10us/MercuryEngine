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

		class GrowableObjectStack : public IObjectAllocator
		{
		public:
			GrowableObjectStack( Scene * scene, size_t max );
			virtual ~GrowableObjectStack();

			bool IsResizable() const override;
			size_t Count() const override;
			bool Available() const override;

			Object * NewObject( std::string name ) override;
			bool DestroyObject( Object * object ) override;
			Object * CopyObject( Object * from, std::string name ) override; 
			void CollectObjects( std::vector< Object * > & objects ) override;
			Object * FindObject( std::string name );

			void Update( IRenderer * renderer, const RenderInfo & renderInfo, CameraCache & cameraCache ) override;
			void Render( IRenderer * renderer, const RenderInfo & renderInfo, const CameraCache & cameras ) override;

		private:
			Scene * m_scene;
			size_t m_max;
			std::list< std::shared_ptr< ObjectStack > > m_stacks;
		};
	}
}