// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Mesh.h>
#include <me/MeshInstanceData.h>

using namespace me;

Mesh::Mesh( me::IRenderer * renderer )
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

void Mesh::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( renderInfo );
	}

	// HACK: TODO:
	m_primitiveList.ComputeBounds( GetBBox() );	 
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
		assert( 0 ); // TODO: Updates to RenderInfo need to be vetted.
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->RenderFrames( myRenderInfo.GetWorldMatrix(), true, true );
	}
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
