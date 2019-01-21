// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/render/Billboard.h>

using namespace me;
using namespace render;

class BillboardInstanceData : public GeometryInstanceData
{
public:
};


Billboard::Billboard( IRenderer * renderer, float size, Effect::ptr effect )
	: m_size{ size }
	, m_primitiveList{ renderer }
{
	float xscalar = 1.0f * size;
	float yscalar = 1.0f * size;
	float zscalar = 0.0f ;

	struct Vertex
	{
		float x, y, z;
		unify::Color diffuse;
	};

	Vertex vbRaw[] =
	{
		// Z-Near...
		{ -xscalar, yscalar, zscalar, unify::ColorRed() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },
		{ xscalar, yscalar, zscalar, unify::ColorGreen() },
		{ xscalar, -yscalar, zscalar, unify::ColorWhite() },
		{ -xscalar, -yscalar, zscalar, unify::ColorBlue() },
	};
	unsigned int numberOfVertices = sizeof( vbRaw ) / sizeof( Vertex );

	auto & bs = m_primitiveList.AddBufferSet();
	bs.AddVertexBuffer( { effect->GetVertexShader()->GetVertexDeclaration(), { { numberOfVertices, vbRaw } }, BufferUsage::Default } );
	bs.AddMethod( RenderMethod::CreateTriangleList( 0, 2, effect ) );
}

Billboard::~Billboard()
{
	Destroy();
}

void Billboard::Destroy()
{
	m_primitiveList.Destroy();
}

const unify::BBox< float > & Billboard::ComputeBounds()
{
	m_primitiveList.ComputeBounds( m_BBox );
	return m_BBox;
}

GeometryInstanceData::ptr Billboard::CreateInstanceData() const
{
	return BillboardInstanceData::ptr {};
}

void Billboard::Update( const UpdateParams & params, GeometryInstanceData * instanceData )
{
	if( instanceData )
	{
		BillboardInstanceData * meshInstanceData = static_cast< BillboardInstanceData * >( instanceData );
	}

	// HACK: TODO:
	m_primitiveList.ComputeBounds( GetBBox() );	 
}

void Billboard::Render( const render::Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed )
{
	Params localParams{ params };
	/*
	unify::Matrix viewMatrix = unify::MatrixIdentity();
	unify::V4< float > positionRow = localParams.renderInfo.GetViewMatrix().GetRow( 3 );
	viewMatrix.SetRow( 3, positionRow );
	localParams.renderInfo.SetViewMatrix( viewMatrix );
	*/

	// TODO: I think we need a smarter, polymorphic MatrixFeed.
	const unify::Matrix & viewMatrix{ params.renderInfo.GetViewMatrix() };
	unify::Matrix lookAt;
	lookAt.LookAtLH( viewMatrix.GetPosition(), unify::V3< float >{ 0.0f, 1.0f, 0.0f } );

	unify::Matrix instance{ unify::MatrixIdentity() };
	render::MatrixFeed matrixFeedLocal( render::MatrixFood_Matrices{ &instance, 1 }, 1 );

	m_primitiveList.Render( localParams, matrixFeedLocal );
}

bool Billboard::IsTrans() const
{
	return m_primitiveList.IsTrans();
}

std::string Billboard::GetSource() const
{
	return "Billboard";
}

bool Billboard::Reload()
{
	return true; // Nothing to do, since we are programmatically generated.
}