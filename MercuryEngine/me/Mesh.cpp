// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Mesh.h>
#include <me/MeshInstanceData.h>

using namespace me;

Mesh::Mesh( me::IRenderer * renderer )
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

GeometryInstanceData * Mesh::CreateInstanceData()
{
	return new MeshInstanceData( m_primitiveList.GetFrameSet(), m_primitiveList.GetAnimationSet() );
}

void Mesh::Destroy()
{
	m_primitiveList.Destroy();
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

void Mesh::Render( RenderParams params, GeometryInstanceData * instanceData, MatrixFeed & matrixFeed )
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
	
	m_primitiveList.Render( params, matrixFeed );
}

unify::Path Mesh::GetSource() const
{
	return m_source;
}

bool Mesh::IsTrans() const
{
	return m_primitiveList.IsTrans();
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
