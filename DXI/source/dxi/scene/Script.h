// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Component.h>
#include <dxi/scripting/IModule.h>

namespace dxi
{
    namespace scene
    {
	    class Script : public Component
	    {
	    public:
		    Script();
		    virtual ~Script();

			void SetModule( scripting::IModule::ptr module );
			scripting::IModule::ptr GetModule();

	    protected:
			scripting::IModule::ptr m_module;
	    };
    }
}