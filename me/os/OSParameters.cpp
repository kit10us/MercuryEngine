// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/os/OSParameters.h>
#include <cassert>

using namespace me;
using namespace os;

OSParameters::OSParameters( unify::Path runPath, unify::Path programPath, std::vector< std::string > arguments )
	: m_programPath{ programPath }
	, m_runPath{ runPath }
	, m_arguments{ arguments }
	, handle{}
	, hInstance{}
	, hPrevInstance{}
	, nCmdShow{}
	, wndProc{}
	, parentHandle{}
{
}

unify::Path OSParameters::GetProgramPath() const
{
	return m_programPath;
}

unify::Path OSParameters::GetRunPath() const
{
	return m_runPath;
}

size_t OSParameters::GetArgumentCount() const
{
	return m_arguments.size();
}

std::string OSParameters::GetArgument( size_t i ) const
{
	assert( i < m_arguments.size() );
	return m_arguments[ i ];
}
