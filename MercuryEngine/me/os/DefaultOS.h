// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/os/IOS.h>
#include <me/game/IGame.h>
#include <me/render/IRenderer.h>
#include <me/render/Display.h>
#include <rm/AssetPaths.h>
#include <list>
#include <vector>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

// Undefine Microsoft clashing defines.
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCommandLine
#undef GetCommandLine
#endif

namespace me
{
	namespace os
	{
		class DefaultOS : public me::os::IOS
		{
		public:
			DefaultOS( me::game::IGame * gameInstance );
			virtual ~DefaultOS();

			me::game::IGame * GetGame() override;

			me::debug::IDebug * Debug() override;
			const me::debug::IDebug * Debug() const override;

			void * Feed( std::string target, void * data ) override;

			std::string GetName() const override;

			std::vector< std::string > GetCommandLine() const override;

			void AddDisplay( me::render::Display display );
			void CreatePendingDisplays( std::string title );
			void CreateDisplay( me::render::Display display, std::string title );
			int RendererCount() const override;
			me::render::IRenderer * GetRenderer( int index ) const override;

			void SetHasFocus( bool hasFocus ) override;
			bool GetHasFocus() const override;

			HINSTANCE GetHInstance() const;
			HWND GetHandle() const;

			void BuildRenderers( std::string title ) override;

			void Startup() override;

			void Shutdown() override;

			LRESULT WndProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );

			rm::AssetPaths::ptr GetAssetPaths() override;

			unify::Path GetProgramPath() const override;

			unify::Path GetRunPath() const override;

		protected:
			me::game::IGame * m_game;
			me::debug::IDebug * m_debug;
			unify::Path m_runPath;
			rm::AssetPaths::ptr m_assetPaths;
		};
	}
}