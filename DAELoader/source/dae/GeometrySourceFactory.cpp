// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/Game.h>
#include <me/render/Mesh.h>

using namespace dae;

GeometrySourceFactory::GeometrySourceFactory( me::render::IRenderer * renderer, util::IEffectSolver * effectSolver )
	: m_renderer( renderer )
	, m_effectSolver( effectSolver )
{
}

me::render::Geometry::ptr GeometrySourceFactory::Produce( unify::Path path, void * data )
{
	dae::Document doc( m_renderer, path, m_effectSolver.get() );
	me::render::Mesh * mesh = new me::render::Mesh( m_renderer );
	const dae::VisualScene & visualScene = *dynamic_cast< const dae::VisualScene* >(doc.Find( doc.GetScene().GetInstanceVisualScene()->GetURL() ));
	visualScene.Build( *mesh );
	mesh->ComputeBounds();
	return me::render::Geometry::ptr( mesh );
}