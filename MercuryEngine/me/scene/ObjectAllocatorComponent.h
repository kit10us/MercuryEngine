// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/SceneComponent.h>
#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		class ObjectAllocatorComponent : public SceneComponent
		{
		public:
			ObjectAllocatorComponent( IOS * os );
			~ObjectAllocatorComponent();

		public: // ISceneComponent...
			void OnAttach( IScene * scene ) override;
			void OnDetach( IScene * scene ) override;
			void OnUpdate( UpdateParams params ) override;
			
			void CollectCameras( RenderGirl & renderGirl ) override;
			void OnRender( RenderGirl & renderGirl ) override;
			
			void OnSuspend() override;
			void OnResume() override;

		public:	// IComponent...
			std::string GetWhat() const override;

		private:
			IObjectAllocator::ptr m_objectStack;		   
		};
	}
}