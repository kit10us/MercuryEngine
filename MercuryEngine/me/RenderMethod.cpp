// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/RenderMethod.h>
#include <me/IRenderer.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/Exception.h>

using namespace me;

PrimitiveType::TYPE PrimitiveType::StringToPrimitiveType( const std::string & type )
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
