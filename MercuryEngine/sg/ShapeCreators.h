// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <sg/ShapeType.h>
#include <unify/Unify.h>
#include <me/PrimitiveList.h>
#include <me/RenderInfo.h>
#include <unify/Parameters.h>
#include <me/Mesh.h>

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
	std::shared_ptr< me::Mesh > CreateShape( me::IRenderer * renderer, unify::Parameters & parameters );
	void CreateShape( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

