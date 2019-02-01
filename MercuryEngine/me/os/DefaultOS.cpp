// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/DefaultDebug.h>
#include <me/os/DefaultOS.h>
#include <me/exception/FailedToCreate.h>
#include <unify/Path.h>
#include <string>

using namespace me;
using namespace os;

DefaultOS::DefaultOS( me::game::IGame * gameInstance, me::os::OSParameters osParameters )
	: m_game{ gameInstance }
	, m_debug{ new debug::DefaultDebug{} }
	, m_assetPaths{ new rm::AssetPaths }
	, m_osParameters{ osParameters }
{
}

DefaultOS::~DefaultOS()
{
}

me::game::IGame * DefaultOS::GetGame()
{
	return m_game;
}

me::debug::IDebug * DefaultOS::Debug()
{
	return m_debug;
}

const me::debug::IDebug * DefaultOS::Debug() const
{
	return m_debug;
}

void * DefaultOS::Feed( std::string target, void * data )
{
	return nullptr;
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

void DefaultOS::SetRenderFactory( me::render::IRendererFactory::ptr renderFactory )
{

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

rm::AssetPaths::ptr DefaultOS::GetAssetPaths()
{
	return m_assetPaths;
}

const me::os::OSParameters * DefaultOS::GetOSParameters() const
{
	return &m_osParameters;
}
