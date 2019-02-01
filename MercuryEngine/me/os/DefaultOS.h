// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/os/IOS.h>
#include <me/game/IGame.h>
#include <me/render/Display.h>
#include <rm/AssetPaths.h>
#include <list>
#include <vector>

namespace me
{
	namespace os
	{
		class DefaultOS : public me::os::IOS
		{
		public:
			DefaultOS( me::game::IGame * gameInstance, me::os::OSParameters parameters );
			virtual ~DefaultOS();

			void CreatePendingDisplays( std::string title );

		public: // me::os::IOS
			me::game::IGame * GetGame() override;
			me::debug::IDebug * Debug() override;
			const me::debug::IDebug * Debug() const override;
			void * Feed( std::string target, void * data ) override;
			void AddDisplay( me::render::Display display ) override;
			void CreateDisplay( me::render::Display display, std::string title );
			int RendererCount() const override;
			me::render::IRenderer * GetRenderer( int index ) const override;
			void SetHasFocus( bool hasFocus ) override;
			bool GetHasFocus() const override;
			void SetRenderFactory( me::render::IRendererFactory::ptr renderFactory ) override;
			void BuildRenderers( std::string title ) override;
			void Startup() override;
			void Shutdown() override;
			rm::AssetPaths::ptr GetAssetPaths() override;
			const me::os::OSParameters * GetOSParameters() const override;

		protected:
			me::game::IGame * m_game;
			me::debug::IDebug * m_debug;
			rm::AssetPaths::ptr m_assetPaths;
			me::os::OSParameters m_osParameters;
		};
	}
}