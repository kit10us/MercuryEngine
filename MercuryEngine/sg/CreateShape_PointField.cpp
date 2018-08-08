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

void sg::CreateShape_PointField( IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float majorRadius = parameters.Get( "majorradius", 1.0f );
	float minorRadius = parameters.Get( "minorradius", 0.0f );
	unsigned int count = parameters.Get< unsigned int >( "count", 100 );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	BufferSet & set = primitiveList.AddBufferSet();

	// Method 1 - Triangle List...
	set.AddMethod( RenderMethod::CreatePointList( 0, count, effect ) );

	// Randomize the vertices positions...
	unify::V3< float > vec, norm;

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

	char * vertices = new char[vd->GetSizeInBytes( 0 ) * count];
	unify::DataLock lock( vertices, vd->GetSizeInBytes( 0 ), count, false, 0 );
	VertexBufferParameters vbParameters{ vd, { { count, vertices } }, bufferUsage };

	float distance;
	unsigned int v;
	for( v = 0; v < count; v++ )
	{
		// Direction...
		norm.x = (float)(rand() % 10000) * 0.0001f * 2.0f + -1.0f;
		norm.y = (float)(rand() % 10000) * 0.0001f * 2.0f + -1.0f;
		norm.z = (float)(rand() % 10000) * 0.0001f * 2.0f + -1.0f;
		norm.Normalize();

		// Distance...
		distance = minorRadius + ((float)(rand() % 10000) * 0.0001f * (majorRadius - minorRadius));
		vec = norm * distance;

		vec += center;

		WriteVertex( *vd, lock, v, positionE, vec );
		WriteVertex( *vd, lock, v, normalE, norm );
		WriteVertex( *vd, lock, v, diffuseE, diffuse );
		WriteVertex( *vd, lock, v, specularE, specular );

		vbParameters.bbox += vec;
	}

	set.AddVertexBuffer( vbParameters );

	delete[] vertices;
}
