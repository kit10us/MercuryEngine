// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/MeshLoaders.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/loader/MeshLoaderX.h>
// TODO: #include <dxi/dae/DAE_Document.h>

#pragma warning( disable : 4996 )

using namespace dxi;

void loader::LoadMesh_DAE( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managers )
{
	/*
	dae::Document doc( filePath, effectSolver, managers );
	const dae::VisualScene & visualScene = *dynamic_cast< const dae::VisualScene* >( doc.Find( doc.GetScene().GetInstanceVisualScene()->GetURL() ) );
	visualScene.Build( primitiveList );
	*/
}
