// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/Game.h>
#include <me/Mesh.h>

using namespace dae;

GeometrySourceFactory::GeometrySourceFactory( me::IRenderer * renderer, util::IEffectSolver * effectSolver )
	: m_renderer( renderer )
	, m_effectSolver( effectSolver )
{
}

me::Geometry::ptr GeometrySourceFactory::Produce( unify::Path path, void * data )
{
	dae::Document doc( m_renderer, path, m_effectSolver.get() );
	me::Mesh * mesh = new me::Mesh( m_renderer );
	const dae::VisualScene & visualScene = *dynamic_cast< const dae::VisualScene* >(doc.Find( doc.GetScene().GetInstanceVisualScene()->GetURL() ));
	visualScene.Build( mesh->GetPrimitiveList() );
	mesh->ComputeBounds();
	return me::Geometry::ptr( mesh );
}