// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/loader/MeshLoaders.h>
#include <dxi/Managers.h>
#include <dxi/geo/Mesh.h>

namespace dxi
{
    namespace loader
    {
		geo::Mesh * LoadMesh( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers managers );

        // This simplifies the process allowing all managers to be pulled from the GeometryManager.
		geo::Geometry::shared_ptr LoadMesh( const unify::Path & filePath, const std::string & name, Managers managers );
    }
}