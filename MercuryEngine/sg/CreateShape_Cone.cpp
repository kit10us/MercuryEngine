// Copyright (c) 2003 - 2013, Quentin S. Smith
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

void sg::CreateShape_Cone( IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	unsigned int segments = parameters.Get< unsigned int >( "segments", 12 );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	float radius = parameters.Get( "radius", 1.0f );
	float height = parameters.Get( "height", 1.0f );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	int textureMode = parameters.Get( "texturemode", TextureMode::Correct );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );
	// TODO: support top and bottom texArea.
	unify::TexArea texArea = parameters.Get< unify::TexArea >( "texarea", unify::TexArea( unify::TexCoords( 0, 0 ), unify::TexCoords( 1, 1 ) ) );
	bool caps = parameters.Get( "caps", true );

	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();

	if( segments < 3 ) segments = 3;

	height *= 0.5f;

	size_t vertexCount = (segments + 1) * 2;
	size_t indexCount = 0;
	if( caps )
	{
		vertexCount += (segments + 2) * 2;
		indexCount = segments * 3 * 1;
	}

	BufferSet & set = primitiveList.AddBufferSet();

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSizeInBytes( 0 ) * vertexCount] );
	unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

	VertexBufferParameters vbParameters{ vd, { { vertexCount, vertices.get() } }, bufferUsage };

	std::vector< Index32 > indices( indexCount );

	// Method 1 - Triangle Strip (sides)
	set.AddMethod( RenderMethod::CreateTriangleStrip( 0, segments * 2, effect ) );

	if( caps )
	{
		set.AddMethod( RenderMethod::CreateTriangleListIndexed( segments + 1, segments * 3, 0, 0, effect ) );
	}

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

	unify::V3< float > pos;
	unify::V3< float > norm;
	float rad = 0;
	float radChange = PI2 / segments;
	unify::TexCoords cChange;
	cChange.u = (texArea.dr.u - texArea.ul.u) / segments;

	// Sides...
	double dRad = 0;
	double dRadChange = PI2 / segments;
	for( unsigned int s = 0; s <= segments; s++ )
	{
		pos = unify::V3< float >( sin( rad ) * radius, -height, cos( rad ) * radius );
		norm = pos;
		norm.Normalize();
		WriteVertex( *vd, lock, (s * 2) + 0, positionE, center );
		WriteVertex( *vd, lock, (s * 2) + 0, normalE, norm );
		WriteVertex( *vd, lock, (s * 2) + 0, texE, unify::TexCoords( cChange.u * s, texArea.dr.v ) );
		WriteVertex( *vd, lock, (s * 2) + 0, diffuseE, diffuse );
		WriteVertex( *vd, lock, (s * 2) + 0, specularE, specular );
		vbParameters.bbox += center;


		pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
		norm = pos;
		norm.Normalize();
		WriteVertex( *vd, lock, (s * 2) + 1, positionE, pos + center );
		WriteVertex( *vd, lock, (s * 2) + 1, normalE, norm );
		WriteVertex( *vd, lock, (s * 2) + 1, texE, unify::TexCoords( cChange.u * s, texArea.ul.v ) );
		WriteVertex( *vd, lock, (s * 2) + 1, diffuseE, diffuse );
		WriteVertex( *vd, lock, (s * 2) + 1, specularE, specular );
		vbParameters.bbox += pos + center;

		if( caps )
		{
			pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
			norm = pos;
			norm.Normalize();
			WriteVertex( *vd, lock, (segments * 2 + 2) + s, positionE, pos + center );
			WriteVertex( *vd, lock, (segments * 2 + 2) + s, normalE, norm );
			WriteVertex( *vd, lock, (segments * 2 + 2) + s, texE, unify::TexCoords( 0.5f + (float)(sin( rad ) * 0.5f), 0.5f + (float)(cos( rad ) * -0.5f) ) );
			WriteVertex( *vd, lock, (segments * 2 + 2) + s, diffuseE, diffuse );
			WriteVertex( *vd, lock, (segments * 2 + 2) + s, specularE, specular );
			vbParameters.bbox += pos + center;
		}
		rad += radChange;
	}

	if( caps )
	{
		for( unsigned int s = 0; s < segments; ++s )
		{
			indices[0 + s * 3] = (segments * 2) + 2 + (s);
			indices[1 + s * 3] = (segments * 2) + 2 + (s)+1;
			indices[2 + s * 3] = (segments * 2) + 2 + segments + 1;
		}

		pos = unify::V3< float >( 0, height, 0 );
		norm = pos;
		norm.Normalize();
		WriteVertex( *vd, lock, segments * 2 + 2 + segments + 1, positionE, pos + center );
		WriteVertex( *vd, lock, segments * 2 + 2 + segments + 1, normalE, norm );
		WriteVertex( *vd, lock, segments * 2 + 2 + segments + 1, texE, unify::TexCoords( 0.5f, 0.5f ) );
		WriteVertex( *vd, lock, segments * 2 + 2 + segments + 1, diffuseE, diffuse );
		WriteVertex( *vd, lock, segments * 2 + 2 + segments + 1, specularE, specular );
		vbParameters.bbox += pos + center;
	}

	set.AddVertexBuffer( vbParameters );

	if( indexCount > 0 )
	{
		set.AddIndexBuffer( { { { indexCount, &indices[0] } }, bufferUsage } );
	}
}
