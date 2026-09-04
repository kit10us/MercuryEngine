// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/script/IScript.h>

using namespace me;
using namespace script;
		
IScript::IScript()
{
}

IScript::~IScript() 
{
}

void IScript::SetOnFailure(std::function< void(IScript*, std::string message) > onFailure)
{
	m_onFailure = onFailure;
}
