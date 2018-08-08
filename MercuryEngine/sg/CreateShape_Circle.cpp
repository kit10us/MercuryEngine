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

// 2D circle in 3d space (filled)
void sg::CreateShape_Circle( IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	unsigned int segments = parameters.Get< unsigned int >( "segments", 12 );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	float radius = parameters.Get( "radius", 1.0f );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	if( segments < 3 ) segments = 3;

	size_t vertexCount = segments + 1;
	size_t indexCount = segments * 3;

	BufferSet & set = primitiveList.AddBufferSet();

	// Method 1 - Fan
	set.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

	std::shared_ptr< unsigned char > vertices( new unsigned char[vertexCount * vd->GetSizeInBytes( 0 )] );
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

	VertexBufferParameters vbParameters{ vd, { { vertexCount, vertices.get() } }, bufferUsage };

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

	// Set the center
	WriteVertex( *vd, lock, 0, positionE, center );
	WriteVertex( *vd, lock, 0, normalE, unify::V3< float >( 0, 1, 0 ) );
	WriteVertex( *vd, lock, 0, texE, unify::TexCoords( 0.5f, 0.5f ) );
	WriteVertex( *vd, lock, 0, diffuseE, diffuse );
	WriteVertex( *vd, lock, 0, specularE, specular );
	vbParameters.bbox += center;

	double dRad = 0;
	double dRadChange = PI2 / segments;
	for( unsigned int v = 1; v <= segments; v++ )
	{

		unify::V3< float > pos( (float)sin( dRad ) * radius, 0, (float)cos( dRad ) * radius );
		pos += center;

		WriteVertex( *vd, lock, v, positionE, pos );
		WriteVertex( *vd, lock, v, normalE, unify::V3< float >( 0, 1, 0 ) );
		WriteVertex( *vd, lock, v, texE, unify::TexCoords( 0.5f + (float)(sin( dRad ) * 0.5), 0.5f + (float)(cos( dRad ) * -0.5) ) );
		WriteVertex( *vd, lock, v, diffuseE, diffuse );
		WriteVertex( *vd, lock, v, specularE, specular );
		vbParameters.bbox += pos;
		dRad += dRadChange;
	}

	set.AddVertexBuffer( vbParameters );

	std::vector< Index32 > indices( indexCount );

	for( size_t s = 0; s < segments; s++ )
	{
		indices[(s * 3) + 0] = 0;
		indices[(s * 3) + 1] = s + 1;
		indices[(s * 3) + 2] = (s < (segments - 1)) ? s + 2 : 1;
	}

	set.AddIndexBuffer( { { { indexCount, &indices[0] } } } );
}
