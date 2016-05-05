// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/shapes/ShapeType.h>
#include <unify/Unify.h>
#include <dxi/PrimitiveList.h>
#include <dxi/RenderInfo.h>
#include <unify/Parameters.h>
#include <dxi/geo/Mesh.h>

namespace dxi
{
	namespace geo
	{
		class Mesh; // Because Mesh includes this file.
	}

    namespace shapes
    {
		geo::Mesh * CreateShape( unify::Parameters & parameters );
	    void CreateShape( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cube( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_PointField( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_PointRing( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_DashRing( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Pyramid( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Circle( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Sphere( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cylinder( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Tube( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Plane( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
	    void CreateShape_Cone( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );
        void CreateShape_BeveledBox( dxi::PrimitiveList & primitiveList, unify::Parameters & parameters );

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