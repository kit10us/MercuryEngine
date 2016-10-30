// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/core/Game.h>
#include <dxi/Mesh.h>

using namespace dae;

GeometrySourceFactory::GeometrySourceFactory( dxi::core::IRenderer * renderer, util::IEffectSolver * effectSolver )
	: m_renderer( renderer )
	, m_effectSolver( effectSolver )
{
}

dxi::Geometry::ptr GeometrySourceFactory::Produce( unify::Path path )
{
	dae::Document doc( m_renderer, path, m_effectSolver.get() );
	dxi::Mesh * mesh = new dxi::Mesh( m_renderer );
	const dae::VisualScene & visualScene = *dynamic_cast< const dae::VisualScene* >(doc.Find( doc.GetScene().GetInstanceVisualScene()->GetURL() ));
	visualScene.Build( mesh->GetPrimitiveList() );
	mesh->ComputeBounds();
	return dxi::Geometry::ptr( mesh );
}