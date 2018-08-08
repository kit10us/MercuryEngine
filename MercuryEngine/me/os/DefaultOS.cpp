// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/os/DefaultDebug.h>
#include <me/os/DefaultOS.h>
#include <me/exception/FailedToCreate.h>
#include <unify/Exception.h>
#include <unify/Path.h>
#include <shellapi.h>
#include <string>

using namespace me;
using namespace os;

#ifdef GetCommandLine
#undef GetCommandLine
#endif

DefaultOS::DefaultOS( me::game::IGame * gameInstance )
	: m_game{ gameInstance }
	, m_debug{ new DefaultDebug{} }
{
	{
		char buffer[MAX_PATH];
		GetCurrentDirectoryA( MAX_PATH, buffer );
		m_runPath = unify::Path( std::string( buffer ) + "/" );
	}

}

DefaultOS::~DefaultOS()
{
}

me::game::IGame * DefaultOS::GetGame()
{
	return m_game;
}

me::os::IDebug * DefaultOS::Debug()
{
	return m_debug;
}

const me::os::IDebug * DefaultOS::Debug() const
{
	return m_debug;
}

void * DefaultOS::Feed( std::string target, void * data )
{
	return nullptr;
}

std::string DefaultOS::GetName() const
{
	return std::string();
}

std::vector< std::string > DefaultOS::GetCommandLine() const
{
	return std::vector< std::string >();
}

void DefaultOS::AddDisplay( render::Display display )
{
	display;
}

void DefaultOS::CreatePendingDisplays( std::string title )
{
	title;
}

void DefaultOS::CreateDisplay( render::Display display, std::string title )
{
	display; title;
}

int DefaultOS::RendererCount() const
{
	return 0;
}

me::render::IRenderer * DefaultOS::GetRenderer( int index ) const
{
	return nullptr;
}

void DefaultOS::SetHasFocus( bool hasFocus )
{
	hasFocus;
}

bool DefaultOS::GetHasFocus() const
{
	return true;
}

HINSTANCE DefaultOS::GetHInstance() const
{
	return 0;
}

HWND DefaultOS::GetHandle() const
{
	return 0;
}

void DefaultOS::BuildRenderers( std::string title )
{
	title;
}

void DefaultOS::Startup()
{
}

void DefaultOS::Shutdown()
{
}

LRESULT DefaultOS::WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

unify::AssetPaths & DefaultOS::GetAssetPaths()
{
	return m_assetPaths;
}

unify::Path DefaultOS::GetProgramPath() const
{
	return unify::Path();
}

unify::Path DefaultOS::GetRunPath() const
{
	return m_runPath;
}
