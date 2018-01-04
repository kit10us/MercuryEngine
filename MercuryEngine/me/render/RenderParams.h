// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/RenderInfo.h>

namespace me
{
	namespace render
	{
		struct Params
		{
			class IRenderer * renderer;
			RenderInfo renderInfo;
			float GetDelta() const { return renderInfo.GetDelta(); };
		};
	}
}