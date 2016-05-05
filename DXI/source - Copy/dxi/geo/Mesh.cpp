// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/geo/Mesh.h>
#include <dxi/RenderState.h>
#include <dxi/core/Game.h>
#include <dxi/MeshInstanceData.h>

using namespace dxi;
using namespace geo;

Mesh::Mesh()
: m_renderNormals( false )
, m_normalLength( 1.0f )
{
}

Mesh::Mesh( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers managers )
: m_renderNormals( false )
, m_normalLength( 1.0f )
{
	CreateFromFile( filePath, effectSolver, managers );
}

Mesh::Mesh( unify::Parameters & parameters )
: m_renderNormals( false )
, m_normalLength( 1.0f )
{
	CreateFromShape( parameters );
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
	m_normalsVB.Release();
	m_primitiveList.Destroy();
	m_created = false;
	m_source.clear();
}

void Mesh::CreateFromFile( const unify::Path & filePath, dae::util::IEffectSolver * effectSolver, Managers managers )
{
	Destroy();

    loader::LoadMesh( filePath, m_primitiveList, effectSolver, managers );
	m_source = filePath.ToString();
	ComputeBounds();
	m_created = true;
}

void Mesh::CreateFromShape( unify::Parameters & parameters )
{
	Destroy();

    shapes::CreateShape( m_primitiveList, parameters );
	m_source = "[SHAPE]";
	m_needsSync = true;
	ComputeBounds();
	m_created = true;
}

void Mesh::Update( unify::Seconds elapsed, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->Update( elapsed );
	}
}

void Mesh::Render( RenderInfo renderInfo, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		renderInfo.SetFrameSetInstance( &meshInstanceData->GetFrameSetInstance() );
	}
	m_primitiveList.Render( renderInfo );

	if( instanceData && renderInfo.IsOptionTrue( RenderOption::RenderFrames ) )
	{
		MeshInstanceData * meshInstanceData = static_cast< MeshInstanceData * >( instanceData );
		meshInstanceData->RenderFrames( renderInfo.GetFinalMatrix(), true, true );
	}

	/* TODO:
	if( m_renderNormals || renderInfo.IsOptionTrue( RenderOption::RenderNormals ) )
	{
		m_normalsVB.Render( RenderMethod( PT_LineList, 0, m_primitiveList.GetNumVertices(), 0 ) );
	}
	*/
}

PrimitiveList & Mesh::GetPrimitiveList()
{
	return m_primitiveList;
}

const unify::BBox< float > & Mesh::ComputeBounds()
{
	return m_primitiveList.ComputeBounds( m_BBox, m_BSphere );
}
