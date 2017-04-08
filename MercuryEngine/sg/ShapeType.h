// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <string>

namespace sg
{
	struct Shape
	{
		enum TYPE
		{
			Cube,			// A cube (Width, Height, Depth)
			PointField,	    // A bunch of points, the least distance from center being between Radius2 and Radius1, number of points is count
			PointRing,	    // A bunch of points in a ring
			DashRing,		// A ring made up of dashed segments (Radius1, Radius2, 
			Pyramid,		// A Pyramid
			Circle,		    // A 2D Circle in 3D space (can be textured, rotated, scaled, etc...)
			Sphere,		    // A sphere
			Cylinder,		// A Cylinder (Radius, Height, Segments=Sides)
			Tube,			// A tube (Radius1, Radius2, Height, Segments=Sides)
			Plane,		    // A 2D plane (Width, Height, Center, Segments, TexCoordsUL, TexCoordsDR)
			Cone,			// A cone (Radius, Height, Segments )
            BeveledBox,
			INVALID,
			COUNT = INVALID
		};
    		
		static TYPE FromString( const std::string type );
	};

    struct CreationAttributes
    {
		CreationAttributes( size_t numberOfVertices, size_t numberOfIndices, size_t numberOfRenders )
			: numberOfVertices( numberOfVertices )
			, numberOfIndices( numberOfIndices )
			, numberOfRenders( numberOfRenders )
		{
		}

        size_t numberOfVertices;
        size_t numberOfIndices;
        size_t numberOfRenders;
    };
}

