// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeType.h>
#include <unify/Unify.h>
#include <dxi/PrimitiveList.h>
#include <dxi/RenderInfo.h>
#include <unify/Parameters.h>
#include <dxi/Mesh.h>

namespace dxi
{
    namespace shapes
    {
		std::shared_ptr< Mesh > CreateShape( core::IRenderer * renderer, unify::Parameters & parameters );
	    void CreateShape( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cube( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_PointField( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_PointRing( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_DashRing( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Pyramid( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Circle( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Sphere( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cylinder( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Tube( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Plane( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cone( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
        void CreateShape_BeveledBox( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );

	    template< class T >
	    class DistanceFunctor
	    {
	    public:
		    typedef std::shared_ptr< DistanceFunctor > shared_ptr;
		    virtual ~DistanceFunctor() {};
		    virtual T operator()( float unitDistance ) = 0;
	    };
    }
}

#include <dxi/shapes/CubeParameters.h>
#include <dxi/shapes/PointFieldParameters.h>
#include <dxi/shapes/PointRingParameters.h>
#include <dxi/shapes/DashRingParameters.h>
#include <dxi/shapes/PyramidParameters.h>
#include <dxi/shapes/CircleParameters.h>
#include <dxi/shapes/SphereParameters.h>
#include <dxi/shapes/CylinderParameters.h>
#include <dxi/shapes/TubeParameters.h>
#include <dxi/shapes/PlaneParameters.h>
#include <dxi/shapes/ConeParameters.h>
#include <dxi/shapes/BeveledBoxParameters.h>