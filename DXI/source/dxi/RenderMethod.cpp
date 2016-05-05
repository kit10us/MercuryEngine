// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/RenderMethod.h>
#include <dxi/core/Game.h>

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
	else if( unify::StringIs( type, "TRIANGLEFAN" ) )
	{
		return PrimitiveType::TriangleFan;
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
, useIB( false )
, vertexBufferIndex( 0 )
, indexBufferIndex( 0 )
{
}

RenderMethod::RenderMethod( PrimitiveType::TYPE primitiveType, unsigned int startVertex, unsigned int vertexCount, unsigned int primitiveCount, Effect::shared_ptr effect, bool useIB, size_t vertexBufferIndex, size_t indexBufferIndex )
: primitiveType( primitiveType )
, startVertex( startVertex )
, baseVertexIndex( 0 )
, minIndex( 0 )
, vertexCount( vertexCount )
, startIndex( 0 )
, primitiveCount( primitiveCount )
, effect( effect )
, useIB( useIB )
, vertexBufferIndex( vertexBufferIndex )
, indexBufferIndex( indexBufferIndex )
{
}

RenderMethod::RenderMethod( PrimitiveType::TYPE primitiveType, int baseVertexIndex, unsigned int minIndex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount, Effect::shared_ptr effect, bool useIB, size_t vertexBufferIndex, size_t indexBufferIndex )
: primitiveType( primitiveType )
, startVertex( 0 )
, baseVertexIndex( baseVertexIndex )
, minIndex( minIndex )
, vertexCount( vertexCount )
, startIndex( startIndex )
, primitiveCount( primitiveCount )
, effect( effect )
, useIB( useIB )
, vertexBufferIndex( vertexBufferIndex )
, indexBufferIndex( indexBufferIndex )
{
}

unsigned int RenderMethod::VertexCountInAFan( unsigned int segmentCount )
{
	return segmentCount + 2;
}

unsigned int RenderMethod::VertexCountInATriangleStrip( unsigned int triangleCount )
{
	return triangleCount + 2;
}

unsigned int RenderMethod::VertexCountInATriangleList( unsigned int triangleCount )
{
	return triangleCount * 3;
}

RenderMethod RenderMethod::CreateFan( unsigned int startVertex, unsigned int segmentCount, Effect::shared_ptr effect )
{	
	return RenderMethod( PrimitiveType::TriangleFan, startVertex, segmentCount + 1, segmentCount, effect, false );
}

RenderMethod RenderMethod::CreateTriangleStrip( unsigned int startVertex, unsigned int segmentCount, Effect::shared_ptr effect )
{
	return RenderMethod( PrimitiveType::TriangleStrip, startVertex, segmentCount + 2, segmentCount, effect, false );
}

RenderMethod RenderMethod::CreatePointList( unsigned int startVertex, unsigned int pointCount, Effect::shared_ptr effect )
{
	return RenderMethod( PrimitiveType::PointList, startVertex, pointCount, pointCount, effect, false );
}

void RenderMethod::Render( RenderInfo & renderInfo ) const
{
	HRESULT hr = S_OK;

	if ( effect )
	{
		effect->Use( renderInfo );
	}

	// Draw Primitive...
	if( useIB == false )
	{
		hr = win::DX::GetDxDevice()->DrawPrimitive( (D3DPRIMITIVETYPE)primitiveType, startVertex, primitiveCount );
	}
	else
	{
		hr = win::DX::GetDxDevice()->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)primitiveType, baseVertexIndex, minIndex, vertexCount, startIndex, primitiveCount );
	}
	OnFailedThrow( hr, "Failed to render vertex buffer!" );
}
