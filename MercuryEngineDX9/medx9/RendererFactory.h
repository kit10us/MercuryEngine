// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/IRenderer.h>
#include <me/render/Display.h>
#include <medx9/DirectX.h>
#include <mewos/WindowsOS.h>
#include <atlbase.h>
#include <memory>

namespace medx9
{
	class RendererFactory : public me::render::IRendererFactory
	{
	public:
		me::render::IRenderer * Produce( me::IOS * os, me::render::Display display, size_t index ) override;
	};
}