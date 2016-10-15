// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Component.h>
#include <dxi/scripting/IModule.h>

namespace dxi
{
    namespace scene
    {
	    class ScriptComponent : public Component
	    {
	    public:
			ScriptComponent();
		    virtual ~ScriptComponent();

			void SetModule( scripting::IModule::ptr module );
			scripting::IModule::ptr GetModule();

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void Update( const RenderInfo & renderInfo ) override;
			void Render( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResune() override;

	    protected:
			scripting::IModule::ptr m_module;
	    };
    }
}