// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/MeshLoaders.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/ASEDocument.h>
#include <dxi/loader/MeshLoaderX.h>
#include <dxi/loader/ShapeLoader.h>
#include <dxi/XMLConvert.h>
#include <unify/String.h>
#include <unify/FrameSet.h>

using namespace dxi;

// It will detect which loader to load...
void loader::LoadMesh( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managers )
{
	/* TODO: broke due to effectSolver.
	if( qlib::file::IsExtension( filePath, ".x" ) )
	{
        MeshLoaderX meshLoader;
        meshLoader.LoadMesh( filePath, primitiveList, effectSolver, managers );
	}
	else */if( filePath.IsExtension( ".xml" ) )
	{
		// Check for an XML shape definition...
		qxml::Document doc( filePath );
		if ( doc.GetRoot()->IsTagName( "shape" ) )
		{
			doc.Destroy();
			ShapeLoader( filePath, primitiveList, managers );
		}
		else
		{
			if ( unify::StringIs( doc.GetRoot()->GetStringAttributeElse( "version", "1.0" ), "1.0" ) || unify::StringIs( doc.GetRoot()->GetStringAttributeElse( "version", "1.0" ), "1" ) )
			{
				LoadMesh_XML_V1_0( filePath, primitiveList, managers );
			}
			else if ( unify::StringIs( doc.GetRoot()->GetStringAttribute( "version" ), "1.1" ) )
			{
				LoadMesh_XML_V1_1( filePath, primitiveList, managers );
			}
			else
			{
				throw unify::Exception( "Failed to determine the correct XML loader for the file version " + doc.GetRoot()->GetStringAttribute( "version" ) );
			}

		}
	}
	else if( filePath.IsExtension( ".ase" ) )
	{
		LoadMesh_ASE( filePath, primitiveList, effectSolver, managers );
	}
	else if( filePath.IsExtension( ".dae" ) )
	{
		LoadMesh_DAE( filePath, primitiveList, effectSolver, managers );
	}
}
