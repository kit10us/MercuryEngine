// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/scene/Script.h>

using namespace dxi;
using namespace scene;

Script::Script()
: Component( "Script" )
{
}

Script::~Script()
{
}

void Script::SetModule( scripting::IModule::ptr module )
{
	m_module = module;
}

scripting::IModule::ptr Script::GetModule()
{
	return m_module;
}
