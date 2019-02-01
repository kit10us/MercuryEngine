// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/SceneComponent.h>
#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		namespace component
		{
			class ObjectAllocatorComponent : public SceneComponent
			{
			public:
				ObjectAllocatorComponent( os::IOS * os );
				~ObjectAllocatorComponent();

			public: // ISceneComponent...
				void OnAttach( IScene * scene ) override;
				void OnDetach( IScene * scene ) override;
				void OnUpdate( const UpdateParams & params ) override;
			
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
}