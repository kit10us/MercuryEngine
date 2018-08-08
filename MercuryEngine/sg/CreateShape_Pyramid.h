// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <unify/Unify.h>
#include <me/render/PrimitiveList.h>
#include <unify/Parameters.h>

namespace sg
{
	void CreateShape_Pyramid( me::render::IRenderer * renderer, me::render::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

#include <sg/PyramidParameters.h>
