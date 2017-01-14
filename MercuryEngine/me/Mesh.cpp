// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Mesh.h>
#include <me/MeshInstanceData.h>

using namespace me;

Mesh::Mesh( const me::IRenderer * renderer )
	: m_primitiveList( renderer )
{
}

Mesh::~Mesh()
{
	Destroy();
}

GeometryInstanceData * Mesh::CreateInstanceData()
{
	return new MeshInstanceData( m_primitiveList.GetFrameSet(), m_primitiveList.GetAnimationSet() );
}

void Mesh::Destroy()
{
	m_primitiveList.Destroy();
}

void Mesh::Update( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( renderInfo );
	}

	// HACK: TODO:
	m_primitiveList.ComputeBounds( GetBBox() );	 
}

void Mesh::Render( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData, MatrixFeed & matrixFeed )
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
	
	m_primitiveList.Render( renderer, renderInfo, matrixFeed );
}

void Mesh::Render( IRenderer * renderer, const RenderInfo & renderInfo, GeometryInstanceData * instanceData, const IMatrixSource * sources, const size_t sources_size, bool contiguous )
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
	
	m_primitiveList.Render( renderer, renderInfo, sources, sources_size, contiguous );
}

PrimitiveList & Mesh::GetPrimitiveList()
{
	return m_primitiveList;
}

const unify::BBox< float > & Mesh::ComputeBounds()
{
	m_primitiveList.ComputeBounds( m_BBox );
	return m_BBox;
}
