// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/render/PrimitiveList.h>
#include <me/render/RenderInfo.h>
#include <me/render/Mesh.h>
#include <unify/Parameters.h>
#include <unify/Unify.h>

#include <sg/ShapeType.h>
#include <sg/CreateShape_Cube.h>
#include <sg/CreateShape_PointField.h>
#include <sg/CreateShape_PointRing.h>
#include <sg/CreateShape_DashRing.h>
#include <sg/CreateShape_Pyramid.h>
#include <sg/CreateShape_Circle.h>
#include <sg/CreateShape_Sphere.h>
#include <sg/CreateShape_Cylinder.h>
#include <sg/CreateShape_Tube.h>
#include <sg/CreateShape_Plane.h>
#include <sg/CreateShape_Cone.h>
#include <sg/CreateShape_BeveledBox.h>

namespace sg
{
	std::shared_ptr< me::render::Mesh > CreateShape( me::render::IRenderer * renderer, unify::Parameters & parameters );
	void CreateShape( me::render::IRenderer * renderer, me::render::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

