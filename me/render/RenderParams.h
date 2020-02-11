// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
			unify::TimeDelta GetDelta() const { return renderInfo.GetDelta(); };
		};
	}
}