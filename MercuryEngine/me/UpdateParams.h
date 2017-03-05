// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>

namespace me
{
	struct UpdateParams
	{
		IRenderer * renderer;
		RenderInfo & renderInfo;
		float GetDelta() const { return renderInfo.GetDelta(); };
	};
}