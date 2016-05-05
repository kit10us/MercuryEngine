// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/MeshLoader.h>
#include <dxi/loader/MeshLoaders.h>
#include <dxi/geo/Mesh.h>

using namespace dxi;

geo::Mesh * loader::LoadMesh( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers managers )
{
	geo::Mesh * mesh = new geo::Mesh;
    loader::LoadMesh( filePath, mesh->GetPrimitiveList(), effectSolver, managers );
    return mesh;
}

geo::Mesh::shared_ptr loader::LoadMesh( const unify::Path & filePath, const std::string & name, Managers managers )
{
	if ( managers.GetGeometryManager()->Exists( name ) )
	{
		return managers.GetGeometryManager()->Find( name );
	}
	
	geo::Mesh * geometry = LoadMesh( filePath, 0, managers );
	return managers.GetGeometryManager()->Add( name, geometry );
}