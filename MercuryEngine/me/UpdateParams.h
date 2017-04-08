// Copyright (c) 2003 - 2014, Quentin S. Smith
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