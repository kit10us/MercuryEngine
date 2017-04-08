// Copyright (c) 2003 - 2014, Quentin S. Smith
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
			const RenderInfo & renderInfo;
			float GetDelta() const { return renderInfo.GetDelta(); };
		};
	}
}