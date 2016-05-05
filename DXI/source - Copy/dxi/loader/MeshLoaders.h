// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/PrimitiveList.h>
#include <dxi/Managers.h>
#include <dxi/RenderMethod.h>
//#include <dxi/dae/util/IEffectSolver.h>

// TODO: Temp to allow compiling.
namespace dae
{
	namespace util
	{
		struct IEffectSolver {};
	}
}

namespace dxi
{
    namespace loader
    {
        // dxi::PrimitiveList loaders...
		void LoadMesh( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managers );
	    void LoadMesh_XML_V1_0( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers );
	    void LoadMesh_XML_V1_1( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers );
		void LoadMesh_ASE( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managers );
		void LoadMesh_DAE( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managesr );
	}
}