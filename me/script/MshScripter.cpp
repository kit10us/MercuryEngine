// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/script/MshScripter.h>

using namespace me;
using namespace script;


MshScripter::MshScripter()
	: m_globals{}
{
}

void MshScripter::SetGlobal(std::string_view name, std::string_view value)
{
	m_globals[std::string(name)] = value;
}

