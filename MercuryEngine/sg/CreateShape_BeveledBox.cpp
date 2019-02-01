// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <sg/CreateShape_BeveledBox.h>
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

void sg::CreateShape_BeveledBox( IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	unify::V3< float > inf;
	unify::V3< float > sup;
	if( parameters.Exists( "inf,sup" ) )
	{
		inf = parameters.Get< unify::V3< float > >( "inf" );
		sup = parameters.Get< unify::V3< float > >( "sup" );
	}
	else
	{
		unify::Size3< float > size( parameters.Get( "size3", unify::Size3< float >( 1.0f, 1.0f, 1.0f ) ) );
		// Divide the dimensions to center the cube
		size.width *= 0.5f;
		size.height *= 0.5f;
		size.depth *= 0.5f;
		inf = unify::V3< float >( -size.width, -size.height, -size.depth );
		sup = unify::V3< float >( size.width, size.height, size.depth );
	}

	unify::Color diffuse = parameters.Get( "diffuse", unify::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::ColorWhite() );
	int textureMode = parameters.Get( "texturemode", TextureMode::Correct );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	const unsigned int verticesPerSide = 4;
	const unsigned int facesPerSide = 2;
	const unsigned int indicesPerSide = 6;
	const unsigned int numberOfSides = 6;
	const unsigned int totalVertices = verticesPerSide * numberOfSides;
	const unsigned int totalIndices = indicesPerSide * numberOfSides;
	const unsigned int totalTriangles = facesPerSide * numberOfSides;

	size_t vertexCount = totalVertices;

	BufferSet & set = primitiveList.AddBufferSet();
	set.SetEffect( effect );

	set.AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 0, totalVertices, 0, totalTriangles, true ) );

	// Set the vertices from the TEMP vertices...
	std::shared_ptr< unsigned char > verticesRaw( new unsigned char[vd->GetSizeInBytes( 0 ) * vertexCount] );
	unify::DataLock lock( verticesRaw.get(), vd->GetSizeInBytes( 0 ), vertexCount, unify::DataLockAccess::ReadWrite, 0 );

	VertexBufferParameters vbParameters{ vd, { { vertexCount, verticesRaw.get() } }, bufferUsage };

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

	// Set the TEMP vertices...
	V vertices[8];

	vertices[0].pos = unify::V3< float >( inf.x, sup.y, inf.z );
	vertices[0].normal.Normalize( vertices[0].pos );
	vertices[0].coords = unify::TexCoords( 0.0f, 0.0f );
	vertices[0].specular = specular;

	vertices[1].pos = unify::V3< float >( inf.x, inf.y, inf.z );
	vertices[1].normal.Normalize( vertices[1].pos );
	vertices[1].coords = unify::TexCoords( 0.0f, 1.0f );
	vertices[1].specular = specular;

	vertices[2].pos = unify::V3< float >( sup.x, sup.y, inf.z );
	vertices[2].normal.Normalize( vertices[2].pos );
	vertices[2].coords = unify::TexCoords( 1.0f, 0.0f );
	vertices[2].specular = specular;

	vertices[3].pos = unify::V3< float >( sup.x, inf.y, inf.z );
	vertices[3].normal.Normalize( vertices[3].pos );
	vertices[3].coords = unify::TexCoords( 1.0f, 1.0f );
	vertices[3].specular = specular;

	vertices[4].pos = unify::V3< float >( sup.x, sup.y, sup.z );
	vertices[4].normal.Normalize( vertices[4].pos );
	vertices[4].coords = unify::TexCoords( 0.0f, 0.0f );
	vertices[4].specular = specular;

	vertices[5].pos = unify::V3< float >( sup.x, inf.y, sup.z );
	vertices[5].normal.Normalize( vertices[5].pos );
	vertices[5].coords = unify::TexCoords( 0.0f, 1.0f );
	vertices[5].specular = specular;

	vertices[6].pos = unify::V3< float >( inf.x, sup.y, sup.z );
	vertices[6].normal.Normalize( vertices[6].pos );
	vertices[6].coords = unify::TexCoords( 1.0f, 0.0f );
	vertices[6].specular = specular;

	vertices[7].pos = unify::V3< float >( inf.x, inf.y, sup.z );
	vertices[7].normal.Normalize( vertices[7].pos );
	vertices[7].coords = unify::TexCoords( 1.0f, 1.0f );
	vertices[7].specular = specular;

	// Translate all points for center.
	for( unsigned int i = 0; i < 8; ++i )
	{
		vertices[i].pos += center;
		vbParameters.bbox += vertices[i].pos;
	}

	// Allow per-vertex diffuse...
	std::vector< unify::Color > diffuses = parameters.Get< std::vector< unify::Color > >( "diffuses", std::vector< unify::Color >() );
	if( diffuses.size() == 8 )
	{
		vertices[0].diffuse = diffuses[0];
		vertices[1].diffuse = diffuses[1];
		vertices[2].diffuse = diffuses[2];
		vertices[3].diffuse = diffuses[3];
		vertices[4].diffuse = diffuses[4];
		vertices[5].diffuse = diffuses[5];
		vertices[6].diffuse = diffuses[6];
		vertices[7].diffuse = diffuses[7];
	}
	// Else, assume 1 (correcting later where necessary)...
	else
	{
		vertices[0].diffuse = diffuse;
		vertices[1].diffuse = diffuse;
		vertices[2].diffuse = diffuse;
		vertices[3].diffuse = diffuse;
		vertices[4].diffuse = diffuse;
		vertices[5].diffuse = diffuse;
		vertices[6].diffuse = diffuse;
		vertices[7].diffuse = diffuse;
	}

	WriteVertex( *vd, lock, { 0, 14, 17 }, *vFormat, &vertices[0] );
	WriteVertex( *vd, lock, { 1, 15, 20 }, *vFormat, &vertices[1] );
	WriteVertex( *vd, lock, { 2, 4, 19 }, *vFormat, &vertices[2] );
	WriteVertex( *vd, lock, { 3, 5, 22 }, *vFormat, &vertices[3] );
	WriteVertex( *vd, lock, { 6, 8, 18 }, *vFormat, &vertices[4] );
	WriteVertex( *vd, lock, { 7, 9, 23 }, *vFormat, &vertices[5] );
	WriteVertex( *vd, lock, { 10, 12, 16 }, *vFormat, &vertices[6] );
	WriteVertex( *vd, lock, { 11, 13, 21 }, *vFormat, &vertices[7] );

	// Set the vertices texture coords...
	switch( textureMode )
	{
	case TextureMode::Correct:
	{
		int v, h;
		for( v = 0; v < 4; v++ )
		{
			for( h = 0; h < 4; h++ )
			{
				WriteVertex( *vd, lock, h + (v * 4), texE, vertices[h].coords );
			}
		}

		WriteVertex( *vd, lock, 16, texE, unify::TexCoords( 0, 0 ) );
		WriteVertex( *vd, lock, 17, texE, unify::TexCoords( 0, 1 ) );
		WriteVertex( *vd, lock, 18, texE, unify::TexCoords( 1, 0 ) );
		WriteVertex( *vd, lock, 19, texE, unify::TexCoords( 1, 1 ) );

		WriteVertex( *vd, lock, 20, texE, unify::TexCoords( 0, 0 ) );
		WriteVertex( *vd, lock, 21, texE, unify::TexCoords( 0, 1 ) );
		WriteVertex( *vd, lock, 22, texE, unify::TexCoords( 1, 0 ) );
		WriteVertex( *vd, lock, 23, texE, unify::TexCoords( 1, 1 ) );
	} break;

	case TextureMode::Wrapped:
	{
		float l1 = 0, l2 = 0.33f;
		float m1 = 0.33f, m2 = 0.66f;
		float r1 = 0.66f, r2 = 1;

		float t1 = 0, t2 = 0.5f;
		float b1 = 0.5f, b2 = 1;

		// Left Side..
		WriteVertex( *vd, lock, 12, texE, unify::TexCoords( l1, b1 ) );
		WriteVertex( *vd, lock, 13, texE, unify::TexCoords( l1, b2 ) );
		WriteVertex( *vd, lock, 14, texE, unify::TexCoords( l2, b1 ) );
		WriteVertex( *vd, lock, 15, texE, unify::TexCoords( l2, b2 ) );

		// Front...
		WriteVertex( *vd, lock, 0, texE, unify::TexCoords( m1, b1 ) );
		WriteVertex( *vd, lock, 1, texE, unify::TexCoords( m1, b2 ) );
		WriteVertex( *vd, lock, 2, texE, unify::TexCoords( m2, b1 ) );
		WriteVertex( *vd, lock, 3, texE, unify::TexCoords( m2, b2 ) );

		// Right Side...
		WriteVertex( *vd, lock, 4, texE, unify::TexCoords( r1, b1 ) );
		WriteVertex( *vd, lock, 5, texE, unify::TexCoords( r1, b2 ) );
		WriteVertex( *vd, lock, 6, texE, unify::TexCoords( r2, b1 ) );
		WriteVertex( *vd, lock, 7, texE, unify::TexCoords( r2, b2 ) );

		// Rear...
		WriteVertex( *vd, lock, 8, texE, unify::TexCoords( l1, t1 ) );
		WriteVertex( *vd, lock, 9, texE, unify::TexCoords( l1, t2 ) );
		WriteVertex( *vd, lock, 10, texE, unify::TexCoords( l2, t1 ) );
		WriteVertex( *vd, lock, 11, texE, unify::TexCoords( l2, t2 ) );

		// Top...
		WriteVertex( *vd, lock, 16, texE, unify::TexCoords( m1, t1 ) );
		WriteVertex( *vd, lock, 17, texE, unify::TexCoords( m1, t2 ) );
		WriteVertex( *vd, lock, 18, texE, unify::TexCoords( m2, t1 ) );
		WriteVertex( *vd, lock, 19, texE, unify::TexCoords( m2, t2 ) );

		// Bottom...
		WriteVertex( *vd, lock, 20, texE, unify::TexCoords( r1, t1 ) );
		WriteVertex( *vd, lock, 21, texE, unify::TexCoords( r1, t2 ) );
		WriteVertex( *vd, lock, 22, texE, unify::TexCoords( r2, t1 ) );
		WriteVertex( *vd, lock, 23, texE, unify::TexCoords( r2, t2 ) );
	} break;
	}

	// Allow per-face diffuse...
	if( diffuses.size() == 6 )
	{
		// Front...
		WriteVertex( *vd, lock, { 0, 1, 2, 3 }, diffuseE, diffuses[1] );

		// Right Side...
		WriteVertex( *vd, lock, { 4, 5, 6, 7 }, diffuseE, diffuses[2] );

		// Rear...
		WriteVertex( *vd, lock, { 8, 9, 10, 11 }, diffuseE, diffuses[3] );

		// Left Side..
		WriteVertex( *vd, lock, { 12, 13, 14, 15 }, diffuseE, diffuses[0] );

		// Top...
		WriteVertex( *vd, lock, { 16, 17, 18, 18 }, diffuseE, diffuses[4] );

		// Bottom...
		WriteVertex( *vd, lock, { 20, 21, 22, 23 }, diffuseE, diffuses[5] );
	}

	set.AddVertexBuffer( vbParameters );

	// Set the Indices..
	Index32 indices[36] =
	{
		// Front
		0, 2, 1,
		2, 3, 1,

		// R Side
		4, 6, 5,
		6, 7, 5,

		// Back
		8, 10, 9,
		10, 11, 9,

		// L Side
		12, 14, 13,
		14, 15, 13,

		// Top
		16, 18, 17,
		18, 19, 17,

		// Bottom
		20, 22, 21,
		22, 23, 21
	};

	set.AddIndexBuffer( { { { totalIndices, indices } }, bufferUsage } );
}