// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/RenderMethod.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/exception/NotImplemented.h>
#include <dxi/exception/Exception.h>

dxi::PrimitiveType::TYPE dxi::PrimitiveType::StringToPrimitiveType( const std::string & type )
{
	if( unify::StringIs( type, "POINTLIST" ) )
	{
		return PrimitiveType::PointList;
	}
	else if( unify::StringIs( type, "LINELIST" ) )
	{
		return PrimitiveType::LineList;
	}
	else if( unify::StringIs( type, "LINESTRIP" ) )
	{
		return PrimitiveType::LineStrip;
	}
	else if( unify::StringIs( type, "TRIANGLELIST" ) )
	{
		return PrimitiveType::TriangleList;
	}
	else if( unify::StringIs( type, "TRIANGLESTRIP" ) )
	{
		return PrimitiveType::TriangleStrip;
	}
	else
	{
		return PrimitiveType::PointList; 
	}
}
													
using namespace dxi;

RenderMethod::RenderMethod()
: startVertex( 0 )
, baseVertexIndex( 0 )
, minIndex( 0 )
, vertexCount( 0 )
, startIndex( 0 )
, primitiveCount( 0 )
, indexCount( 0 )
, useIB( false )
, vertexBufferIndex( 0 )
, indexBufferIndex( 0 )
{
}

RenderMethod::RenderMethod( PrimitiveType::TYPE type, unsigned int startVertex, unsigned int vertexCount, unsigned int primitiveCount, Effect::ptr effect, bool useIB, size_t vertexBufferIndex, size_t indexBufferIndex )
: primitiveType( type )
, startVertex( startVertex )
, baseVertexIndex( 0 )
, minIndex( 0 )
, vertexCount( vertexCount )
, startIndex( 0 )
, primitiveCount( primitiveCount )
, indexCount( 0 )
, effect( effect )
, useIB( useIB )
, vertexBufferIndex( vertexBufferIndex )
, indexBufferIndex( indexBufferIndex )
{
}

RenderMethod::RenderMethod( PrimitiveType::TYPE type, int baseVertexIndex, unsigned int minIndex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount, Effect::ptr effect, bool useIB, size_t vertexBufferIndex, size_t indexBufferIndex )
: primitiveType( type )
, startVertex( 0 )
, baseVertexIndex( baseVertexIndex )
, minIndex( minIndex )
, vertexCount( vertexCount )
, startIndex( startIndex )
, primitiveCount( primitiveCount )
, indexCount( 0 )
, effect( effect )
, useIB( useIB )
, vertexBufferIndex( vertexBufferIndex )
, indexBufferIndex( indexBufferIndex )
{
}

unsigned int RenderMethod::VertexCountInATriangleStrip( unsigned int triangleCount )
{
	return triangleCount + 2;
}

unsigned int RenderMethod::VertexCountInATriangleList( unsigned int triangleCount )
{
	return triangleCount * 3;
}

RenderMethod RenderMethod::CreateTriangleStrip( unsigned int startVertex, unsigned int segmentCount, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::TriangleStrip;
	method.startVertex = startVertex;
	method.baseVertexIndex = 0;
	method.minIndex = 0;
	method.vertexCount = VertexCountInATriangleStrip( segmentCount );
	method.startIndex = 0;
	method.primitiveCount = segmentCount;
	method.indexCount = 0;
	method.effect = effect;
	method.useIB = false;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}

RenderMethod RenderMethod::CreateTriangleList( unsigned int startVertex, unsigned int triangleCount, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::TriangleList;
	method.startVertex = startVertex;
	method.baseVertexIndex = 0;
	method.minIndex = 0;
	method.vertexCount = VertexCountInATriangleList( triangleCount );
	method.startIndex = 0;
	method.primitiveCount = triangleCount;
	method.indexCount = 0;
	method.effect = effect;
	method.useIB = false;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}

RenderMethod RenderMethod::CreatePointList( unsigned int startVertex, unsigned int pointCount, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::PointList;
	method.startVertex = startVertex;
	method.baseVertexIndex = 0;
	method.minIndex = 0;
	method.vertexCount = pointCount;
	method.startIndex = 0;
	method.primitiveCount = pointCount;
	method.indexCount = 0;
	method.effect = effect;
	method.useIB = false;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}

RenderMethod RenderMethod::CreateLineList( unsigned int startVertex, unsigned int lineCount, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::LineList;
	method.startVertex = startVertex;
	method.baseVertexIndex = 0;
	method.minIndex = 0;
	method.vertexCount = lineCount * 2;
	method.startIndex = 0;
	method.primitiveCount = lineCount;
	method.indexCount = 0;
	method.effect = effect;
	method.useIB = false;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}

// indexCount, startIndex, baseVertexIndex
RenderMethod RenderMethod::CreateTriangleStripIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::TriangleStrip;
	method.startVertex = 0;
	method.baseVertexIndex = baseVertexIndex;
	method.minIndex = 0;
	method.vertexCount = vertexCount;
	method.startIndex = startIndex;
	method.primitiveCount = indexCount - 2;
	method.indexCount = indexCount;
	method.effect = effect;
	method.useIB = true;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}

RenderMethod RenderMethod::CreateTriangleListIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex, Effect::ptr effect )
{
	RenderMethod method;
	method.primitiveType = PrimitiveType::TriangleList;
	method.startVertex = 0;
	method.baseVertexIndex = baseVertexIndex;
	method.minIndex = 0;
	method.vertexCount = vertexCount;
	method.startIndex = startIndex;
	method.primitiveCount = indexCount / 3;
	method.indexCount = indexCount;
	method.effect = effect;
	method.useIB = true;
	method.vertexBufferIndex = 0;
	method.indexBufferIndex = 0;
	return method;
}


void RenderMethod::Render( const RenderInfo & renderInfo ) const
{
	HRESULT hr = S_OK;

	if ( effect )
	{
		effect->Use( renderInfo );
	}

#if defined( DIRECTX9 )
	D3DPRIMITIVETYPE dxPrimitiveType {};
	switch( primitiveType )
	{
	case PrimitiveType::PointList: dxPrimitiveType = D3DPT_POINTLIST; break;
	case PrimitiveType::LineList: dxPrimitiveType = D3DPT_LINELIST; break;
	case PrimitiveType::LineStrip: dxPrimitiveType = D3DPT_LINESTRIP; break;
	case PrimitiveType::TriangleList: dxPrimitiveType = D3DPT_TRIANGLELIST;	break;
	case PrimitiveType::TriangleStrip: dxPrimitiveType = D3DPT_TRIANGLESTRIP;  break;
	}
							   
	auto dxRenderer = dynamic_cast<const win::DXRenderer *>(renderInfo.GetRenderer());
	auto dxDevice = dxRenderer->GetDxDevice();
		
	// Draw Primitive...
	if( useIB == false )
	{
		hr = dxDevice->DrawPrimitive( dxPrimitiveType, startVertex, primitiveCount );
	}
	else
	{
		hr = dxDevice->DrawIndexedPrimitive( dxPrimitiveType, baseVertexIndex, minIndex, vertexCount, startIndex, primitiveCount );
	}
	OnFailedThrow( hr, "Failed to render vertex buffer!" );
#elif defined( DIRECTX11 )
	auto dxRenderer = dynamic_cast<const win::DXRenderer *>(renderInfo.GetRenderer());
	auto dxDevice = dxRenderer->GetDxDevice();
	auto dxContext = dxRenderer->GetDxContext();
	
	D3D11_PRIMITIVE_TOPOLOGY topology{};
	switch( primitiveType )
	{
	case PrimitiveType::PointList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; 
		break;
	case PrimitiveType::LineList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; 
		break;
	case PrimitiveType::LineStrip: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; 
		break;
	case PrimitiveType::TriangleList: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	
		break;
	case PrimitiveType::TriangleStrip: 
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;  
		break;
	}
	dxContext->IASetPrimitiveTopology( topology );

	if( useIB == false )
	{
		dxContext->Draw( vertexCount,  startVertex );
	}
	else
	{
		dxContext->DrawIndexed( indexCount, startIndex, baseVertexIndex );
	}
#endif
}
