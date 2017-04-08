// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <unify/Unify.h>
#include <me/render/PrimitiveList.h>
#include <unify/Parameters.h>

namespace sg
{
	void CreateShape_Plane( me::render::IRenderer * renderer, me::render::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

#include <sg/PlaneParameters.h>
