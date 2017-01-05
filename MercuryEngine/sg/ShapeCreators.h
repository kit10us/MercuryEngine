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

namespace sg
{
	std::shared_ptr< me::Mesh > CreateShape( me::IRenderer * renderer, unify::Parameters & parameters );
	void CreateShape( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_PointField( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_PointRing( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_DashRing( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Pyramid( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Circle( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Sphere( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Cylinder( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Tube( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Plane( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
	void CreateShape_Cone( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
    void CreateShape_BeveledBox( me::IRenderer * renderer, me::PrimitiveList & primitiveList, unify::Parameters & parameters );
}

#include <sg/PointFieldParameters.h>
#include <sg/PointRingParameters.h>
#include <sg/DashRingParameters.h>
#include <sg/PyramidParameters.h>
#include <sg/CircleParameters.h>
#include <sg/SphereParameters.h>
#include <sg/CylinderParameters.h>
#include <sg/TubeParameters.h>
#include <sg/PlaneParameters.h>
#include <sg/ConeParameters.h>
#include <sg/BeveledBoxParameters.h>