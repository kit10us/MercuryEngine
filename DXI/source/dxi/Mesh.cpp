// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Mesh.h>
#include <dxi/core/Game.h>
#include <dxi/MeshInstanceData.h>

using namespace dxi;

Mesh::Mesh()
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

void Mesh::Update( unify::Seconds elapsed, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( elapsed );
	}
}

void Mesh::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	RenderInfo myRenderInfo( renderInfo );

	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		myRenderInfo.SetFrameSetInstance( &meshInstanceData->GetFrameSetInstance() );
	}
	m_primitiveList.Render( myRenderInfo );

	if( instanceData && myRenderInfo.IsOptionTrue( RenderOption::RenderFrames ) )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->RenderFrames( myRenderInfo.GetFinalMatrix(), true, true );
	}
}

PrimitiveList & Mesh::GetPrimitiveList()
{
	return m_primitiveList;
}

const unify::BBox< float > & Mesh::ComputeBounds()
{
	return m_primitiveList.ComputeBounds( m_BBox );
}
