// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <unify/Unify.h>
#include <me/render/PrimitiveList.h>
#include <unify/Parameters.h>

namespace sg
{
	void CreateShape_Circle( me::render::IRenderer * renderer, me::render::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

#include <sg/CircleParameters.h>
