// Copyright (c) 2002 - 2013, Quentin S. Smith
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
			void OnAttach( Scene * scene ) override;
			void OnDetach( Scene * scene ) override;
			void OnInit( Scene * scene ) override;
			void OnStart( Scene * scene ) override;
			void OnUpdate( Scene * scene, UpdateParams params ) override;
			
			void CollectCameras( RenderGirl & renderGirl ) override;
			void OnRender( Scene * scene, RenderGirl & renderGirl ) override;
			
			void OnSuspend() override;
			void OnResume() override;

		public:	// IComponent...
			std::string GetWhat() const override;

		private:
			IObjectAllocator::ptr m_objectStack;		   
		};
	}
}