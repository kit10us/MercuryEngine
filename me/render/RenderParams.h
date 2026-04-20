// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/RenderInfo.h>
#include <unify/TimeDelta.h>

namespace me::render
{
	class IRenderer;
}

namespace me
{
	namespace render
	{
		struct Params
		{
			IRenderer * renderer;
			RenderInfo renderInfo;
			unify::TimeDelta GetDelta() const { return renderInfo.GetDelta(); };
		};
	}
}