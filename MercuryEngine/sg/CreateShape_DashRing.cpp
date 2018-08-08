// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <sg/ShapeCreators.h>
#include <me/render/RenderMethod.h>
#include <me/render/TextureMode.h>
#include <me/render/BufferUsage.h>
#include <me/render/VertexUtil.h>
#include <me/exception/NotImplemented.h>
#include <unify/String.h>
#include <unify/Size3.h>
#include <unify/TexArea.h>
#include <unify/Angle.h>

using namespace me;
using namespace render;

const float PI = 3.14159265358979f;
const float PI2 = 6.28318530717959f;
const std::string DefaultBufferUsage = "Default";

void sg::CreateShape_DashRing( IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float radiusOuter = parameters.Get( "majorradius", 1.0f );
	float radiusInner = parameters.Get( "minorradius", 0.9f );
	unsigned int count = parameters.Get< unsigned int >( "count", 12 );	// Number of dashes
	float fSize = parameters.Get( "size1", 0.5f );	// Unit size of visible part of dash (0.0 to 1.0)
	float definition = parameters.Get( "definition", 4.0f );		// Definition of each dash
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	int verticesPerSegment = (int)((definition + 1) * 2);
	int indicesPerSegment = (int)(definition * 6);
	int facesPerSegment = (int)(definition * 2);

	unsigned int totalVertices = verticesPerSegment * count;
	unsigned int totalIndices = indicesPerSegment * count;
	unsigned int totalTriangles = facesPerSegment * count;

	BufferSet & set = primitiveList.AddBufferSet();

	// Method 1 - Triangle List...
	set.AddMethod( RenderMethod::CreateTriangleListIndexed( totalVertices, totalIndices, 0, 0, effect ) );

	char * vertices = new char[vd->GetSizeInBytes( 0 ) * totalVertices];
	unify::DataLock lock( vertices, vd->GetSizeInBytes( 0 ), totalVertices, false, 0 );
	VertexBufferParameters vbParameters{ vd, { { count, vertices } }, bufferUsage };

	unsigned short stream = 0;

	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement normalE = CommonVertexElement::Normal( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	VertexElement specularE = CommonVertexElement::Specular( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	class V
	{
	public:
		unify::V3< float > pos;
		unify::V3< float > normal;
		unify::Color diffuse;
		unify::Color specular;
		unify::TexCoords coords;
	};
	qjson::Object jsonFormat;
	jsonFormat.Add( { "Position", "Float3" } );
	jsonFormat.Add( { "Normal", "Float3" } );
	jsonFormat.Add( { "Diffuse", "Color" } );
	jsonFormat.Add( { "Specular", "Color" } );
	jsonFormat.Add( { "TexCoord", "TexCoord" } );
	VertexDeclaration::ptr vFormat( new VertexDeclaration( jsonFormat ) );

	// Create all the segments (clockwise from top)
	unify::V3< float > vOuter, vInner, vNorm;
	unify::TexCoords coordsInner, coordsOuter;

	float fRadChange = ((PI2 / count) * fSize) / definition;
	float fRadChangeSeg = (PI2 / count) * (1.0f - fSize);

	float fRad = 0.0f;
	for( unsigned int segment = 0; segment < count; segment++ )
	{
		// Set the starting vector for this segment
		int vertex = segment * verticesPerSegment;
		for( int d = 0; d < (int)(definition + 1); d++ )
		{
			coordsOuter = unify::TexCoords( cosf( fRad ) * 1.0f, sinf( fRad ) * 1.0f );
			coordsInner = unify::TexCoords( cosf( fRad ) * (radiusInner / radiusOuter), sinf( fRad ) * (radiusInner / radiusOuter) );

			vOuter = unify::V3< float >( cosf( fRad ) * radiusOuter, 0, sinf( fRad ) * radiusOuter );
			vInner = unify::V3< float >( cosf( fRad ) * radiusInner, 0, sinf( fRad ) * radiusInner );

			// Outter Radius...
			WriteVertex( *vd, lock, vertex, positionE, vOuter + center );
			WriteVertex( *vd, lock, vertex, normalE, unify::V3< float >( 0, 1, 0 ) );
			WriteVertex( *vd, lock, vertex, diffuseE, diffuse );
			WriteVertex( *vd, lock, vertex, specularE, specular );
			WriteVertex( *vd, lock, vertex, texE, coordsOuter );

			// Inner Radius...
			WriteVertex( *vd, lock, vertex + 1, positionE, vInner + center );
			WriteVertex( *vd, lock, vertex + 1, normalE, unify::V3< float >( 0, 1, 0 ) );
			WriteVertex( *vd, lock, vertex + 1, diffuseE, diffuse );
			WriteVertex( *vd, lock, vertex + 1, specularE, specular );
			WriteVertex( *vd, lock, vertex + 1, texE, coordsInner );

			vbParameters.bbox += vOuter + center;
			vbParameters.bbox += vInner + center;

			// Move to the next ver
			vertex += 2;

			//Move the vectors (rotate)
			if( d != (int)(definition) ) fRad += fRadChange;
		}
		fRad += fRadChangeSeg;
	}

	set.AddVertexBuffer( vbParameters );
	delete[] vertices;

	// Create the index list...
	std::vector< Index32 > indices( totalIndices );
	Index32 io = 0, vo = 0;	// Index and vertex offset
	for( unsigned int segment = 0; segment < count; ++segment )
	{
		for( int iDef = 0; iDef < (int)definition; ++iDef )
		{
			indices[io++] = 0 + vo;
			indices[io++] = 1 + vo;
			indices[io++] = 2 + vo;

			indices[io++] = 1 + vo;
			indices[io++] = 3 + vo;
			indices[io++] = 2 + vo;

			vo += 2;
		}
		vo += 2;
	}

	IndexBufferParameters ibParameters{ { { totalIndices, &indices[0] } }, bufferUsage };
	set.AddIndexBuffer( ibParameters);
}
