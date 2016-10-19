// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/ObjectComponent.h>
#include <dxi/scripting/IModule.h>

namespace dxi
{
    namespace scene
    {
	    class ScriptComponent : public ObjectComponent
	    {
	    public:
			ScriptComponent( core::IOS * os );
		    virtual ~ScriptComponent();

			std::string GetName() const override;

			void SetModule( scripting::IModule::ptr module );
			scripting::IModule::ptr GetModule();

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void OnUpdate( const RenderInfo & renderInfo ) override;
			void OnRender( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResume() override;

	    protected:
			scripting::IModule::ptr m_module;
	    };
    }
}