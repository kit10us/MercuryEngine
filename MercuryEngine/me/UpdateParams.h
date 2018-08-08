// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/RenderInfo.h>

namespace me
{
	struct UpdateParams
	{
		render::IRenderer * renderer;
		render::RenderInfo & renderInfo;
		float GetDelta() const { return renderInfo.GetDelta(); };
	};
}