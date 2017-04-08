// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/Mesh.h>
#include <me/render/MeshInstanceData.h>

using namespace me;
using namespace render;

Mesh::Mesh( IRenderer * renderer )
	: m_primitiveList{ renderer }
{
}

Mesh::Mesh( unify::Path source, IRenderer * renderer )
	: m_source{ source }
	, m_primitiveList{ renderer }
{
}

Mesh::~Mesh()
{
	Destroy();
}

void Mesh::Destroy()
{
	m_primitiveList.Destroy();
}

Skeleton * Mesh::GetSkeleton()
{
	return &m_skeleton;
}

const Skeleton * Mesh::GetSkeleton() const
{
	return &m_skeleton;
}

void Mesh::SetSkeletonEffect( Effect::ptr effect )
{
	m_skeletonEffect = effect;
}

const unify::BBox< float > & Mesh::ComputeBounds()
{
	m_primitiveList.ComputeBounds( m_BBox );
	return m_BBox;
}

PrimitiveList & Mesh::GetPrimitiveList()
{
	return m_primitiveList;
}

GeometryInstanceData * Mesh::CreateInstanceData()
{
	return new MeshInstanceData( m_primitiveList.GetFrameSet(), m_primitiveList.GetAnimationSet() );
}

void Mesh::Update( UpdateParams params, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( params.renderInfo );
	}

	// HACK: TODO:
	m_primitiveList.ComputeBounds( GetBBox() );	 
}

void Mesh::Render( render::Params params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed )
{
	// TODO:
	/*
	RenderInfo myRenderInfo( renderInfo );

	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		myRenderInfo.SetFrameSetInstance( &meshInstanceData->GetFrameSetInstance() );
	}
	m_primitiveList.Render( myRenderInfo, list );

	if( instanceData && myRenderInfo.IsOptionTrue( RenderOption::RenderFrames ) )
	{
		assert( 0 ); // TODO: Updates to RenderInfo need to be vetted.
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->RenderFrames( myRenderInfo.GetWorldMatrix(), true, true );
	}
	*/
	
	if( !m_skeleton.Empty() && m_skeletonEffect )
	{
		m_skeleton.Render( params, matrixFeed, m_skeletonEffect, 0.05f, unify::Color::ColorBlue() );
	}
	//else
	{
		m_primitiveList.Render( params, matrixFeed );
	}
}

unify::Path Mesh::GetSource() const
{
	return m_source;
}

bool Mesh::IsTrans() const
{
	return m_primitiveList.IsTrans();
}

bool Mesh::Reload()
{
	return false; // TODO: Will need to keep the factory function for this (which is possible).
}