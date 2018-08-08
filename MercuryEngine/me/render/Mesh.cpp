// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/Mesh.h>
#include <me/render/MeshInstanceData.h>

using namespace me;
using namespace render;

Mesh::Mesh( IRenderer * renderer )
	: m_primitiveList{ renderer }
	, m_renderMesh{ true }
{
}

Mesh::Mesh( unify::Path source, IRenderer * renderer )
	: m_source{ source }
	, m_primitiveList{ renderer }
	, m_renderMesh{ true }
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

void Mesh::SetRenderMesh( bool render )
{
	m_renderMesh = render;
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

void Mesh::Update( const UpdateParams & params, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( params.renderInfo );
	}

	// HACK: TODO:
	m_primitiveList.ComputeBounds( GetBBox() );	 
}

void Mesh::Render( const render::Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed )
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

	if( m_skeleton.Empty() )
	{
		if( m_renderMesh )
		{
			m_primitiveList.Render( params, matrixFeed );
		}
	}
	else
	{
		// Draw skeleton, if we have an effect for it...
		if( m_skeletonEffect )
		{
			m_skeleton.Render( params, matrixFeed, m_skeletonEffect, 0.05f, unify::Color::ColorBlue() );
		}

		/*
		// Draw meshes one at a time...
		unify::Matrix matrix;
		MatrixFeed localMatrixFeed;
		while( matrixFeed.Consume( &matrix, 1 ) )
		{
			localMatrixFeed
			for( size_t i = 0; i < m_jointFinalMatrix.size(); i++ )
			{
				unify::Matrix matrixFinal =
					m_jointFinalMatrix[i]
					* matrix;
				render::MatrixFeed feed( render::MatrixFood_Matrices{ &matrixFinal, 1 } );
				m_jointGeo->Render( params, nullptr, feed );
			}
		}
		matrixFeed.Restart();
		*/

		if( m_renderMesh )
		{
			m_primitiveList.Render( params, matrixFeed );
		}
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