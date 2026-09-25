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
				unify::Result<> OnAttach( IScene * scene ) override;
				unify::Result<> OnDetach( IScene * scene ) override;
				unify::Result<> OnUpdate( const UpdateParams & params ) override;
			
				void CollectCameras( RenderGirl & renderGirl ) override;
				unify::Result<> OnRender( RenderGirl & renderGirl ) override;
			
				unify::Result<> OnSuspend() override;
				unify::Result<> OnResume() override;

			public:	// IComponent...
				std::string GetWhat() const override;

			private:
				IObjectAllocator::ptr m_objectStack;		   
			};
		}
	}
}