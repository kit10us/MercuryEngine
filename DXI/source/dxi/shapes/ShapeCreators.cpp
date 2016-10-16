// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/shapes/ShapeCreators.h>
#include <dxi/RenderMethod.h>
#include <unify/String.h>
#include <unify/Size3.h>
#include <unify/TexArea.h>
#include <dxi/exception/NotImplemented.h>
#include <unify/Angle.h>

using namespace dxi;

const float PI = 3.14159265358979f;
const float PI2 = 6.28318530717959f;
const std::string DefaultBufferUsage = "Default";

std::shared_ptr< Mesh > shapes::CreateShape( core::IRenderer * renderer, unify::Parameters & parameters )
{
	std::shared_ptr< Mesh > mesh( new Mesh( renderer ) );
	CreateShape( renderer, mesh->GetPrimitiveList(), parameters );
	//mesh->GetPrimitiveList().ComputeBounds( mesh->GetBBox() );
	return mesh;
}

void shapes::CreateShape( core::IRenderer * renderer, dxi::PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	Shape::TYPE type = parameters.Get< Shape::TYPE >( "type" );

	switch( type )
	{
	case Shape::Cube:
		CreateShape_Cube( renderer, primitiveList, parameters );
		break;
	case Shape::PointField:
		CreateShape_PointField( renderer, primitiveList, parameters );
		break;
	case Shape::PointRing:
		CreateShape_PointRing( renderer, primitiveList, parameters );
		break;
	case Shape::DashRing:
		CreateShape_DashRing( renderer, primitiveList, parameters );
		break;
	case Shape::Pyramid:
		CreateShape_Pyramid( renderer, primitiveList, parameters );
		break;
	case Shape::Circle:
		CreateShape_Circle( renderer, primitiveList, parameters );
		break;
	case Shape::Sphere:
		CreateShape_Sphere( renderer, primitiveList, parameters );
		break;
	case Shape::Cylinder:
		CreateShape_Cylinder( renderer, primitiveList, parameters );
		break;
	case Shape::Tube:
		CreateShape_Tube( renderer, primitiveList, parameters );
		break;
	case Shape::Plane:
		CreateShape_Plane( renderer, primitiveList, parameters );
		break;
	case Shape::Cone:
		CreateShape_Cone( renderer, primitiveList, parameters );
		break;
	case Shape::BeveledBox:
		CreateShape_BeveledBox( renderer, primitiveList, parameters );
	default:
		throw unify::Exception( "Invalid Mesh m_shapeType! Failed to create m_shapeType!" );
	}
}

void shapes::CreateShape_Cube( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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

	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	BufferSet & set = primitiveList.AddBufferSet();

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod::CreateTriangleListIndexed( totalVertices, totalIndices, 0, 0, effect ) );

	VertexBuffer & vb = set.GetVertexBuffer();

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
		vb.GetBBox() += vertices[i].pos;
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

	char * verticesFinal = new char[totalVertices * vd->GetSize()];

	unify::DataLock lock( verticesFinal, vd->GetSize(), totalVertices, false );

	unsigned short stream = 0;

	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement normalE = CommonVertexElement::Normal( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	VertexElement specularE = CommonVertexElement::Specular( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	vd->WriteVertex( lock, { 0, 14, 17 }, vFormat, (void*)&vertices[0] );
	vd->WriteVertex( lock, { 1, 15, 20 }, vFormat, (void*)&vertices[1] );
	vd->WriteVertex( lock, { 2, 4, 19 }, vFormat, (void*)&vertices[2] );
	vd->WriteVertex( lock, { 3, 5, 22 }, vFormat, (void*)&vertices[3] );
	vd->WriteVertex( lock, { 6, 8, 18 }, vFormat, (void*)&vertices[4] );
	vd->WriteVertex( lock, { 7, 9, 23 }, vFormat, (void*)&vertices[5] );
	vd->WriteVertex( lock, { 10, 12, 16 }, vFormat, (void*)&vertices[6] );
	vd->WriteVertex( lock, { 11, 13, 21 }, vFormat, (void*)&vertices[7] );

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
				vd->WriteVertex( lock, h + (v * 4), texE, vertices[h].coords );
			}
		}

		vd->WriteVertex( lock, 16, texE, unify::TexCoords( 0, 0 ) );
		vd->WriteVertex( lock, 17, texE, unify::TexCoords( 0, 1 ) );
		vd->WriteVertex( lock, 18, texE, unify::TexCoords( 1, 0 ) );
		vd->WriteVertex( lock, 19, texE, unify::TexCoords( 1, 1 ) );

		vd->WriteVertex( lock, 20, texE, unify::TexCoords( 0, 0 ) );
		vd->WriteVertex( lock, 21, texE, unify::TexCoords( 0, 1 ) );
		vd->WriteVertex( lock, 22, texE, unify::TexCoords( 1, 0 ) );
		vd->WriteVertex( lock, 23, texE, unify::TexCoords( 1, 1 ) );
	} break;

	case TextureMode::Wrapped:
	{
		float l1 = 0, l2 = 0.33f;
		float m1 = 0.33f, m2 = 0.66f;
		float r1 = 0.66f, r2 = 1;

		float t1 = 0, t2 = 0.5f;
		float b1 = 0.5f, b2 = 1;

		// Left Side..
		vd->WriteVertex( lock, 12, texE, unify::TexCoords( l1, b1 ) );
		vd->WriteVertex( lock, 13, texE, unify::TexCoords( l1, b2 ) );
		vd->WriteVertex( lock, 14, texE, unify::TexCoords( l2, b1 ) );
		vd->WriteVertex( lock, 15, texE, unify::TexCoords( l2, b2 ) );

		// Front...
		vd->WriteVertex( lock, 0, texE, unify::TexCoords( m1, b1 ) );
		vd->WriteVertex( lock, 1, texE, unify::TexCoords( m1, b2 ) );
		vd->WriteVertex( lock, 2, texE, unify::TexCoords( m2, b1 ) );
		vd->WriteVertex( lock, 3, texE, unify::TexCoords( m2, b2 ) );

		// Right Side...
		vd->WriteVertex( lock, 4, texE, unify::TexCoords( r1, b1 ) );
		vd->WriteVertex( lock, 5, texE, unify::TexCoords( r1, b2 ) );
		vd->WriteVertex( lock, 6, texE, unify::TexCoords( r2, b1 ) );
		vd->WriteVertex( lock, 7, texE, unify::TexCoords( r2, b2 ) );

		// Rear...
		vd->WriteVertex( lock, 8, texE, unify::TexCoords( l1, t1 ) );
		vd->WriteVertex( lock, 9, texE, unify::TexCoords( l1, t2 ) );
		vd->WriteVertex( lock, 10, texE, unify::TexCoords( l2, t1 ) );
		vd->WriteVertex( lock, 11, texE, unify::TexCoords( l2, t2 ) );

		// Top...
		vd->WriteVertex( lock, 16, texE, unify::TexCoords( m1, t1 ) );
		vd->WriteVertex( lock, 17, texE, unify::TexCoords( m1, t2 ) );
		vd->WriteVertex( lock, 18, texE, unify::TexCoords( m2, t1 ) );
		vd->WriteVertex( lock, 19, texE, unify::TexCoords( m2, t2 ) );

		// Bottom...
		vd->WriteVertex( lock, 20, texE, unify::TexCoords( r1, t1 ) );
		vd->WriteVertex( lock, 21, texE, unify::TexCoords( r1, t2 ) );
		vd->WriteVertex( lock, 22, texE, unify::TexCoords( r2, t1 ) );
		vd->WriteVertex( lock, 23, texE, unify::TexCoords( r2, t2 ) );
	} break;
	}

	// Allow per-face diffuse...
	if( diffuses.size() == 6 )
	{
		// Front...
		vd->WriteVertex( lock, { 0, 1, 2, 3 }, diffuseE, diffuses[1] );

		// Right Side...
		vd->WriteVertex( lock, { 4, 5, 6, 7 }, diffuseE, diffuses[2] );

		// Rear...
		vd->WriteVertex( lock, { 8, 9, 10, 11 }, diffuseE, diffuses[3] );

		// Left Side..
		vd->WriteVertex( lock, { 12, 13, 14, 15 }, diffuseE, diffuses[0] );

		// Top...
		vd->WriteVertex( lock, { 16, 17, 18, 19 }, diffuseE, diffuses[4] );

		// Bottom...
		vd->WriteVertex( lock, { 20, 21, 22, 23 }, diffuseE, diffuses[5] );
	}

	vb.Create( totalVertices, vd, verticesFinal, bufferUsage );
	delete[] verticesFinal;

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

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( totalIndices, indices, bufferUsage );
}

void shapes::CreateShape_PointField( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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
	VertexBuffer & vb = set.GetVertexBuffer();
	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();

	// Method 1 - Triangle List...
	rb.AddMethod( RenderMethod::CreatePointList( 0, count, effect ) );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	char * vertices = new char[vd->GetSize() * count];
	unify::DataLock lock( vertices, vd->GetSize(), count, false );

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

		vd->WriteVertex( lock, v, positionE, vec );
		vd->WriteVertex( lock, v, normalE, norm );
		vd->WriteVertex( lock, v, diffuseE, diffuse );
		vd->WriteVertex( lock, v, specularE, specular );

		vb.GetBBox() += vec;
	}

	vb.Create( count, vd, vertices, bufferUsage );

	delete[] vertices;
}

void shapes::CreateShape_PointRing( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float majorRadius = parameters.Get( "majorradius", 0.5f );	// Size radius
	float minorRadius = parameters.Get( "minorradius", 1.0f );	// Radius of ring
	unsigned int count = parameters.Get< unsigned int >( "count", 100 );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	BufferSet & set = primitiveList.AddBufferSet();
	VertexBuffer & vb = set.GetVertexBuffer();
	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();

	rb.AddMethod( RenderMethod::CreatePointList( 0, count, effect ) );

	unify::V3< float > vec;
	unify::V2< float > vPos2;	// Initial position ( by radius1 )
	unify::V3< float > vPos3;
	unify::V3< float > norm;

	char * vertices = new char[vd->GetSize() * count];
	unify::DataLock lock( vertices, vd->GetSize(), count, false );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	for( unsigned int v = 0; v < count; v++ )
	{
		// Generate initial position...
		vPos2.x = (float)(rand() % 10000) * 0.0001f;
		vPos2.y = (float)(rand() % 10000) * 0.0001f;
		vPos2.Normalize();
		vPos2 *= majorRadius;

		// Allow inversions...
		if( (rand() % 2) ) vPos2.x *= -1.0f;
		if( (rand() % 2) ) vPos2.y *= -1.0f;

		vec.x = vPos2.x;
		vec.z = vPos2.y;
		vec.y = 0;

		// Generate from Radius2
		vPos3.x = (float)(rand() % 10000) * 0.0001f;
		vPos3.y = (float)(rand() % 10000) * 0.0001f;
		vPos3.z = (float)(rand() % 10000) * 0.0001f;
		vPos3.Normalize();
		vPos3 *= minorRadius;

		// Allow inversions...
		if( (rand() % 2) ) vPos3.x *= -1.0f;
		if( (rand() % 2) ) vPos3.y *= -1.0f;
		if( (rand() % 2) ) vPos3.z *= -1.0f;

		vec += vPos3;

		norm = vec;
		norm.Normalize();

		vec += center;

		vd->WriteVertex( lock, v, positionE, vec );
		vd->WriteVertex( lock, v, normalE, norm );
		vd->WriteVertex( lock, v, diffuseE, diffuse );
		vd->WriteVertex( lock, v, specularE, specular );   
		vb.GetBBox() += vec;
	}
	vb.Create( count, vd, vertices, bufferUsage );
	delete[] vertices;
}

void shapes::CreateShape_DashRing( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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
	VertexBuffer & vb = set.GetVertexBuffer();

	// Method 1 - Triangle List...
	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod::CreateTriangleListIndexed( totalVertices, totalIndices, 0, 0, effect ) );

	char * vertices = new char[vd->GetSize() * totalVertices];
	unify::DataLock lock( vertices, vd->GetSize(), totalVertices, false );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

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
			vd->WriteVertex( lock, vertex, positionE, vOuter + center );
			vd->WriteVertex( lock, vertex, normalE, unify::V3< float >( 0, 1, 0 ) );
			vd->WriteVertex( lock, vertex, diffuseE, diffuse );
			vd->WriteVertex( lock, vertex, specularE, specular );
			vd->WriteVertex( lock, vertex, texE, coordsOuter );

			// Inner Radius...
			vd->WriteVertex( lock, vertex + 1, positionE, vInner + center );
			vd->WriteVertex( lock, vertex + 1, normalE, unify::V3< float >( 0, 1, 0 ) );
			vd->WriteVertex( lock, vertex + 1, diffuseE, diffuse );
			vd->WriteVertex( lock, vertex + 1, specularE, specular );
			vd->WriteVertex( lock, vertex + 1, texE, coordsInner );

			vb.GetBBox() += vOuter + center;
			vb.GetBBox() += vInner + center;

			// Move to the next ver
			vertex += 2;

			//Move the vectors (rotate)
			if( d != (int)(definition) ) fRad += fRadChange;
		}
		fRad += fRadChangeSeg;
	}

	vb.Create( totalVertices, vd, vertices, bufferUsage );
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

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( totalIndices, (Index32*)&indices[0], bufferUsage );
}

void shapes::CreateShape_Pyramid( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	unify::Size3< float > size( parameters.Get( "size3", unify::Size3< float >( 1.0f, 1.0f, 1.0f ) ) );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	int textureMode = parameters.Get( "texturemode", TextureMode::Correct );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );
	size_t vertexCount = 16;
	size_t indexCount = 18;

	// Divide the dimensions to center the cube
	size *= 0.5f;

	BufferSet & set = primitiveList.AddBufferSet();
	VertexBuffer & vb = set.GetVertexBuffer();

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	// Set the TEMP vertices...
	V vertices[5];

	// Top point
	vertices[0].pos = unify::V3< float >( 0, size.height, 0 );
	vertices[0].normal.Normalize( vertices[0].pos );
	vertices[0].coords = unify::TexCoords( 0.5f, 0.5f );
	vertices[0].specular = specular;

	// FL
	vertices[1].pos = unify::V3< float >( -size.width, -size.height, -size.depth );
	vertices[1].normal.Normalize( vertices[1].pos );
	vertices[1].coords = unify::TexCoords( 0.0f, 1.0f );
	vertices[1].specular = specular;

	// FR
	vertices[2].pos = unify::V3< float >( size.width, -size.height, -size.depth );
	vertices[2].normal.Normalize( vertices[2].pos );
	vertices[2].coords = unify::TexCoords( 1, 1 );
	vertices[2].specular = specular;

	// BL
	vertices[3].pos = unify::V3< float >( size.width, -size.height, size.depth );
	vertices[3].normal.Normalize( vertices[3].pos );
	vertices[3].coords = unify::TexCoords( 0, 1 );
	vertices[3].specular = specular;

	// BR
	vertices[4].pos = unify::V3< float >( -size.width, -size.height, size.depth );
	vertices[4].normal.Normalize( vertices[4].pos );
	vertices[4].coords = unify::TexCoords( 1, 1 );
	vertices[4].specular = specular;

	// Translate all points for center.
	for( unsigned int i = 0; i < 5; ++i )
	{
		vertices[i].pos += center;
		vb.GetBBox() += vertices[i].pos;
	}

	// Allow per-vertex diffuse...
	std::vector< unify::Color > diffuses = parameters.Get< std::vector< unify::Color > >( "diffuses", std::vector< unify::Color >() );
	if( diffuses.size() == 5 )
	{
		vertices[0].diffuse = diffuses[0];
		vertices[1].diffuse = diffuses[1];
		vertices[2].diffuse = diffuses[2];
		vertices[3].diffuse = diffuses[3];
		vertices[4].diffuse = diffuses[4];
	}
	// Else, assume 1 (correcting later where necessary)...
	else
	{
		vertices[0].diffuse = diffuse;
		vertices[1].diffuse = diffuse;
		vertices[2].diffuse = diffuse;
		vertices[3].diffuse = diffuse;
		vertices[4].diffuse = diffuse;
	}

	// Set the vertices from the TEMP vertices...
	std::shared_ptr< unsigned char > verticesRaw( new unsigned char[vd->GetSize() * vertexCount] );
	unify::DataLock lock( verticesRaw.get(), vd->GetSize(), vertexCount, false );

	vd->WriteVertex( lock, { 0, 3, 6, 9 }, vFormat, &vertices[0] );

	vd->WriteVertex( lock, { 1, 11, 12 }, vFormat, &vertices[1] );

	vd->WriteVertex( lock, { 2, 4, 14 }, vFormat, &vertices[2] );

	vd->WriteVertex( lock, { 5, 7, 15 }, vFormat, &vertices[3] );

	vd->WriteVertex( lock, { 8, 10, 13 }, vFormat, &vertices[4] );

	// Set the vertices texture coords...
	switch( textureMode )
	{
	case TextureMode::Correct:
	{
		// Sides
		for( int s = 0; s < 4; s++ )
		{
			vd->WriteVertex( lock, (s * 3), texE, vertices[0].coords );
			vd->WriteVertex( lock, (s * 3) + 1, texE, vertices[1].coords );
			vd->WriteVertex( lock, (s * 3) + 2, texE, vertices[2].coords );
		}

		// Bottom
		vd->WriteVertex( lock, 12, texE, unify::TexCoords( 0, 0 ) );
		vd->WriteVertex( lock, 13, texE, unify::TexCoords( 0, 1 ) );
		vd->WriteVertex( lock, 14, texE, unify::TexCoords( 1, 0 ) );
		vd->WriteVertex( lock, 15, texE, unify::TexCoords( 1, 1 ) );

	} break;

	case TextureMode::Wrapped:
	{
		float l1 = 0, l2 = 0.33f;
		float m1 = 0.33f, m2 = 0.66f;
		float r1 = 0.66f, r2 = 1;

		float t1 = 0, t2 = 0.5f;
		float b1 = 0.5f, b2 = 1;

		// Left Side...
		vd->WriteVertex( lock, 9, texE, unify::TexCoords( 0.1667f, b1 ) );
		vd->WriteVertex( lock, 10, texE, unify::TexCoords( l1, b2 ) );
		vd->WriteVertex( lock, 11, texE, unify::TexCoords( l2, b2 ) );

		// Front...
		vd->WriteVertex( lock, 0, texE, unify::TexCoords( 0.5f, b1 ) );
		vd->WriteVertex( lock, 1, texE, unify::TexCoords( m1, b2 ) );
		vd->WriteVertex( lock, 2, texE, unify::TexCoords( m2, b2 ) );

		// Right Side...
		vd->WriteVertex( lock, 3, texE, unify::TexCoords( 0.8333f, b1 ) );
		vd->WriteVertex( lock, 4, texE, unify::TexCoords( r1, b2 ) );
		vd->WriteVertex( lock, 5, texE, unify::TexCoords( r2, b2 ) );

		// Rear...
		vd->WriteVertex( lock, 6, texE, unify::TexCoords( 0.1667f, t1 ) );
		vd->WriteVertex( lock, 7, texE, unify::TexCoords( l1, t2 ) );
		vd->WriteVertex( lock, 8, texE, unify::TexCoords( l2, t2 ) );

		// Bottom...
		vd->WriteVertex( lock, 12, texE, unify::TexCoords( r1, t1 ) );
		vd->WriteVertex( lock, 13, texE, unify::TexCoords( r1, t2 ) );
		vd->WriteVertex( lock, 14, texE, unify::TexCoords( r2, t1 ) );
		vd->WriteVertex( lock, 15, texE, unify::TexCoords( r2, t2 ) );
	}break;
	}

	vb.Create( vertexCount, vd, verticesRaw.get(), bufferUsage );

	// Set the Indices..
	Index32 indices[18] =
	{
		// Front
		0, 2, 1,

		// R Side
		3, 5, 4,

		// Back
		6, 8, 7,

		// L Side
		9, 11, 10,

		// Bottom
		12, 14, 13,
		14, 15, 13
	};

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( indexCount, indices, bufferUsage );
}


// 2D circle in 3d space (filled)
void shapes::CreateShape_Circle( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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
	VertexBuffer & vb = set.GetVertexBuffer();
	IndexBuffer & ib = set.GetIndexBuffer();

	// Method 1 - Fan
	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

	std::shared_ptr< unsigned char > vertices( new unsigned char[vertexCount * vd->GetSize()] );
	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	// Set the center
	vd->WriteVertex( lock, 0, positionE, center );
	vd->WriteVertex( lock, 0, normalE, unify::V3< float >( 0, 1, 0 ) );
	vd->WriteVertex( lock, 0, texE, unify::TexCoords( 0.5f, 0.5f ) );
	vd->WriteVertex( lock, 0, diffuseE, diffuse );
	vd->WriteVertex( lock, 0, specularE, specular );
	vb.GetBBox() += center;

	double dRad = 0;
	double dRadChange = PI2 / segments;
	for( unsigned int v = 1; v <= segments; v++ )
	{

		unify::V3< float > pos( (float)sin( dRad ) * radius, 0, (float)cos( dRad ) * radius );
		pos += center;

		vd->WriteVertex( lock, v, positionE, pos );
		vd->WriteVertex( lock, v, normalE, unify::V3< float >( 0, 1, 0 ) );
		vd->WriteVertex( lock, v, texE, unify::TexCoords( 0.5f + (float)(sin( dRad ) * 0.5), 0.5f + (float)(cos( dRad ) * -0.5) ) );
		vd->WriteVertex( lock, v, diffuseE, diffuse );
		vd->WriteVertex( lock, v, specularE, specular );
		vb.GetBBox() += pos;
		dRad += dRadChange;
	}

	vb.Create( vertexCount, vd, vertices.get(), bufferUsage );

	std::vector< Index32 > indices( indexCount );

	for( size_t s = 0; s < segments; s++ )
	{
		indices[(s * 3) + 0] = 0;
		indices[(s * 3) + 1] = s + 1;
		indices[(s * 3) + 2] = (s < (segments - 1)) ? s + 2 : 1;
	}

	ib.Create( indexCount, &indices[0] );
}

void shapes::CreateShape_Sphere( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float radius = parameters.Get( "radius", 1.0f );
	unsigned int segments = parameters.Get< unsigned int >( "segments", 12 );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	if( segments < 4 ) segments = 4;

	bool bStrip = true;

	// TRIANGLE LIST version:
	if( !bStrip )
	{	// LIST VERSION

		int iFacesH = (int)segments;
		int iFacesV = (int)iFacesH / 2;

		int vertexCount = (iFacesH + 1) * (iFacesV + 1);

		int iNumFaces = iFacesH * iFacesV * 2;	// Twice as many to count for triangles
		int indexCount = iNumFaces * 3;			// Three indices to a triangle

		BufferSet & set = primitiveList.AddBufferSet();
		VertexBuffer & vb = set.GetVertexBuffer();

		// Method 1 - Triangle List...
		RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
		rb.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

		std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );
		unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

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
		dxi::VertexDeclaration vFormat( renderer, jsonFormat );

		unify::V3< float > vec, norm;

		// Set the vertices...
		float fRadH, fRadV;
		int iVert = 0;
		int v, h;
		for( v = 0; v < (iFacesV + 1); v++ )
		{
			fRadV = (PI / iFacesV) * v;

			for( h = 0; h < (iFacesH + 1); h++ )
			{
				fRadH = (PI2 / iFacesH) * h;

				vec = unify::V3< float >(
					(cosf( fRadH )	* sinf( fRadV )	* radius),
					(1 * cosf( fRadV )	* radius),
					(sinf( fRadH )	* sinf( fRadV )	* radius)
					);

				norm = vec;
				norm.Normalize();

				vec += center;
				vd->WriteVertex( lock, iVert, positionE, vec );
				vd->WriteVertex( lock, iVert, normalE, norm );
				vd->WriteVertex( lock, iVert, diffuseE, diffuse );
				vd->WriteVertex( lock, iVert, specularE, specular );
				vd->WriteVertex( lock, iVert, texE, unify::TexCoords( h * (1.0f / iFacesH), v * (1.0f / iFacesV) ) );
				vb.GetBBox() += vec;
				iVert++;
			}
		}

		vb.Create( vertexCount, vd, vertices.get(), bufferUsage );


		// Indices...
		std::vector< Index32 > indices( indexCount );
		Index32 io = 0;
		for( v = 0; v < iFacesV; v++ )
		{
			for( h = 0; h < iFacesH; h++ )
			{
				//						V							H
				indices[io++] = (Index32)((v * (iFacesH + 1)) + h);
				indices[io++] = (Index32)((v * (iFacesH + 1)) + h + 1);
				indices[io++] = (Index32)(((v + 1) * (iFacesH + 1)) + h);

				indices[io++] = (Index32)((v * (iFacesH + 1)) + h + 1);
				indices[io++] = (Index32)(((v + 1) * (iFacesH + 1)) + h + 1);
				indices[io++] = (Index32)(((v + 1) * (iFacesH + 1)) + h);
			}
		}

		IndexBuffer & ib = set.GetIndexBuffer();
		ib.Create( indexCount, (Index32*)indices[0], bufferUsage );
	}

	else
	{	// STRIP VERSION
		int iRows = (int)segments;
		int iColumns = (int)iRows / 2;
		int vertexCount = (iRows + 1) * (iColumns + 1);
		int indexCount = (iColumns * (2 * (iRows + 1))) + (((iColumns - 1) * 2));

		BufferSet & set = primitiveList.AddBufferSet();

		// Method 1 - Triangle Strip...
		RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
		VertexBuffer & vb = set.GetVertexBuffer();
		RenderMethod renderMethod( RenderMethod::CreateTriangleStripIndexed( vertexCount, indexCount, 0, 0, effect ) );
		rb.AddMethod( renderMethod );

		std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );

		unsigned short stream = 0;

		VertexElement positionE = CommonVertexElement::Position( stream );
		VertexElement normalE = CommonVertexElement::Normal( stream );
		VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
		VertexElement specularE = CommonVertexElement::Specular( stream );
		VertexElement texE = CommonVertexElement::TexCoords( stream );
								
		unify::V3< float > vec, norm;
		unify::TexCoords coords;

		// Set the vertices...
		unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );
		float fRadH, fRadV;
		int iVert = 0;
		int v, h;
		for( v = 0; v < (iColumns + 1); v++ )
		{
			fRadV = (PI / (iColumns)) * v;

			for( h = 0; h < (iRows + 1); h++ )
			{
				fRadH = (PI2 / iRows) * h;

				vec = unify::V3< float >(
					cosf( fRadH )	* sinf( fRadV )	* radius,	// X
					-1 * cosf( fRadV )	* radius,	// Y
					sinf( fRadH )	* sinf( fRadV )	* radius	// Z
					);

				norm = vec;
				norm.Normalize();

				vec += center;

				coords = unify::TexCoords( h * (1.0f / iRows), 1 - v * (1.0f / iColumns) );

				vd->WriteVertex( lock, iVert, positionE, vec );
				vd->WriteVertex( lock, iVert, normalE, norm );
				vd->WriteVertex( lock, iVert, diffuseE, diffuse );
				vd->WriteVertex( lock, iVert, specularE, specular );
				vd->WriteVertex( lock, iVert, texE, coords );
				vb.GetBBox() += vec;
				iVert++;
			}
		}

		vb.Create( vertexCount, vd, vertices.get(), bufferUsage );

		std::vector< Index32 > indices( indexCount );

		// Indices...
		Index32 io = 0;
		int segmentmentsH = iRows + 1;	// Number of segments
		for( v = 0; v < iColumns; v++ )
		{
			for( h = 0; h < segmentmentsH; h++ )
			{
				indices[io++] = (Index32)((segmentmentsH * v) + h);
				indices[io++] = (Index32)((segmentmentsH * (v + 1)) + h);
			}
			if( v < (iColumns - 1) )
			{
				indices[io++] = (Index32)((segmentmentsH * (v + 2)) - 1);
				indices[io++] = (Index32)(segmentmentsH * (v + 1));
			}
		}

		IndexBuffer & ib = set.GetIndexBuffer();
		ib.Create( indexCount, (Index32*)&indices[0], bufferUsage );
	}
}

void shapes::CreateShape_Cylinder( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float radius = parameters.Get( "radius", 0.5f );
	unsigned int segments = parameters.Get< unsigned int >( "segments", 12 );
	float height = parameters.Get( "height", 1.0f );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	unify::TexArea texArea = parameters.Get< unify::TexArea >( "texarea", unify::TexArea( unify::TexCoords( 0, 0 ), unify::TexCoords( 1, 1 ) ) );
	// TODO: support top and bottom texArea.
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	bool caps = parameters.Get( "caps", true );
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	if( segments < 3 ) segments = 3;

	height *= 0.5f;

	size_t vertexCount = (segments + 1) * 2;
	size_t indexCount = 0;
	if( caps )
	{
		vertexCount += (segments + 2) * 2;
		indexCount = segments * 3 * 2;
	}

	BufferSet & set = primitiveList.AddBufferSet();
	VertexBuffer & vb = set.GetVertexBuffer();
	IndexBuffer & ib = set.GetIndexBuffer();

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );
	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

	std::vector< Index32 > indices( indexCount );

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();

	// Method 1 - Triangle Strip (sides)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 0, segments * 2, effect ) );

	if( caps )
	{
		rb.AddMethod( RenderMethod::CreateTriangleListIndexed( segments + 1, segments * 3, 0, 0, effect ) );
		rb.AddMethod( RenderMethod::CreateTriangleListIndexed( segments + 1, segments * 3, segments * 3, 0, effect ) );
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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

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
		vd->WriteVertex( lock, (s * 2) + 0, positionE, pos + center );
		vd->WriteVertex( lock, (s * 2) + 0, normalE, norm );
		vd->WriteVertex( lock, (s * 2) + 0, texE, unify::TexCoords( cChange.u * s, texArea.dr.v ) );
		vd->WriteVertex( lock, (s * 2) + 0, diffuseE, diffuse );
		vd->WriteVertex( lock, (s * 2) + 0, specularE, specular );
		vb.GetBBox() += pos + center;

		pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
		norm = pos;
		norm.Normalize();
		vd->WriteVertex( lock, (s * 2) + 1, positionE, pos + center );
		vd->WriteVertex( lock, (s * 2) + 1, normalE, norm );
		vd->WriteVertex( lock, (s * 2) + 1, texE, unify::TexCoords( cChange.u * s, texArea.ul.v ) );
		vd->WriteVertex( lock, (s * 2) + 1, diffuseE, diffuse );
		vd->WriteVertex( lock, (s * 2) + 1, specularE, specular );
		vb.GetBBox() += pos + center;

		if( caps )
		{
			pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
			norm = pos;
			norm.Normalize();
			vd->WriteVertex( lock, (segments * 2 + 2) + s, positionE, pos + center );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, normalE, norm );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, texE, unify::TexCoords( 0.5f + (float)(sin( rad ) * 0.5f), 0.5f + (float)(cos( rad ) * -0.5f) ) );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, diffuseE, diffuse );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, specularE, specular );
			vb.GetBBox() += pos + center;

			pos = unify::V3< float >( cos( rad ) * radius, -height, sin( rad ) * radius );
			norm = pos;
			norm.Normalize();
			vd->WriteVertex( lock, (segments * 2 + 2) + (segments + 2) + s, positionE, pos + center );
			vd->WriteVertex( lock, (segments * 2 + 2) + (segments + 2) + s, normalE, norm );
			vd->WriteVertex( lock, (segments * 2 + 2) + (segments + 2) + s, texE, unify::TexCoords( 0.5f + (float)(sin( rad ) * 0.5f), 0.5f + (float)(cos( rad ) * -0.5f) ) );
			vd->WriteVertex( lock, (segments * 2 + 2) + (segments + 2) + s, diffuseE, diffuse );
			vd->WriteVertex( lock, (segments * 2 + 2) + (segments + 2) + s, specularE, specular );
			vb.GetBBox() += pos + center;
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

			indices[(segments * 3) + 0 + s * 3] = (segments * 2) + 2 + segments + 2 + (s);
			indices[(segments * 3) + 2 + s * 3] = (segments * 2) + 2 + segments + 2 + (s)+1;
			indices[(segments * 3) + 1 + s * 3] = (segments * 2) + 2 + segments + 2 + segments + 1;
		}

		pos = unify::V3< float >( 0, height, 0 );
		norm = pos;
		norm.Normalize();
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, positionE, pos + center );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, normalE, norm );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, texE, unify::TexCoords( 0.5f, 0.5f ) );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, diffuseE, diffuse );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, specularE, specular );
		vb.GetBBox() += pos + center;

		pos.y = -height;
		norm = pos;
		norm.Normalize();
		vd->WriteVertex( lock, segments * 2 + 2 + ((segments + 1) * 2) + 1, positionE, pos + center );
		vd->WriteVertex( lock, segments * 2 + 2 + ((segments + 1) * 2) + 1, normalE, norm );
		vd->WriteVertex( lock, segments * 2 + 2 + ((segments + 1) * 2) + 1, texE, unify::TexCoords( 0.5f, 0.5f ) );
		vd->WriteVertex( lock, segments * 2 + 2 + ((segments + 1) * 2) + 1, diffuseE, diffuse );
		vd->WriteVertex( lock, segments * 2 + 2 + ((segments + 1) * 2) + 1, specularE, specular );
		vb.GetBBox() += pos + center;
	}

	vb.Create( vertexCount, vd, vertices.get(), bufferUsage );

	if( indexCount > 0 )
	{
		ib.Create( indexCount, &indices[0], bufferUsage );
	}
}

void shapes::CreateShape_Tube( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	float outer = parameters.Get( "majorradius", 0.5f );
	float inner = parameters.Get( "minorradius", 0.3f );
	unsigned int segments = parameters.Get< unsigned int >( "segments", 12 );
	float height = parameters.Get( "height", 1.0f );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );

	if( segments < 3 ) segments = 3;

	size_t vertexCount = RenderMethod::VertexCountInATriangleStrip( segments * 2 ) * 4;

	// Height is distance from origin/center.
	height *= 0.5f;

	// a segment is made up of two triangles... segments * 2 = NumTriangles
	BufferSet & set = primitiveList.AddBufferSet();
	VertexBuffer & vb = set.GetVertexBuffer();

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );

	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	unsigned int trianglesPerSide = segments * 2;
	unsigned int verticesPerSide = segments * 2 + 2;

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();

	// Method 1 - Triangle Strip (Top)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 0 * verticesPerSide, trianglesPerSide, effect ) );

	// Method 2 - Triangle Strip (Bottom)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 1 * verticesPerSide, trianglesPerSide, effect ) );

	// Method 3 - Triangle Strip (Outside)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 2 * verticesPerSide, trianglesPerSide, effect ) );

	// Method 4 - Triangle Strip (Inside)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 3 * verticesPerSide, trianglesPerSide, effect ) );

	V vertex;
	float radiansChange = PI2 / segments;
	float ratioT = inner / outer;
	for( unsigned int v = 0; v < (segments + 1); v++ )
	{
		float radians = radiansChange * v;

		unify::V2< float > coord( sin( radians ), cos( radians ) );

		// Method 1 - Triangle Strip (Top)
		// Outside edge
		vertex.pos = unify::V3< float >( coord.x * outer, height, coord.y * outer );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( 0, 1, 0 );
		vertex.coords = unify::TexCoords( 0.5f + (coord.x * 0.5f), 0.5f + (coord.y * -0.5f) );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (0 * verticesPerSide) + (v * 2), vFormat, &vertex );
		vb.GetBBox() += vertex.pos;


		// Inside edge
		vertex.pos = unify::V3< float >( coord.x * inner, height, coord.y * inner );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( 0, 1, 0 );
		vertex.coords = unify::TexCoords( 0.5f + (coord.x * 0.5f * ratioT), 0.5f + (coord.y * -0.5f * ratioT) );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (0 * verticesPerSide) + (v * 2) + 1, vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Method 2 - Triangle Strip (Bottom)
		// Outside edge
		vertex.pos = unify::V3< float >( coord.x * outer, -height, coord.y * outer );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( 0, -1, 0 );
		vertex.coords = unify::TexCoords( 0.5f + (coord.x * 0.5f), 0.5f + (coord.y * -0.5f) );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (1 * verticesPerSide) + (v * 2), vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Inside edge
		vertex.pos = unify::V3< float >( coord.x * inner, -height, coord.y * inner );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( 0, -1, 0 );
		vertex.coords = unify::TexCoords( 0.5f + (coord.x * 0.5f * ratioT), 0.5f + (coord.y * -0.5f * ratioT) );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (1 * verticesPerSide) + (v * 2) + 1, vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Method 3 - Triangle Strip (Outside)
		// Top
		vertex.pos = unify::V3< float >( coord.x * outer, height, coord.y * outer );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( coord.x, 0, coord.y );
		vertex.coords = unify::TexCoords( (1.0f / segments) * v, 0 );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (2 * verticesPerSide) + (v * 2), vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Bottom
		vertex.pos = unify::V3< float >( coord.x * outer, -height, coord.y * outer );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( coord.x, 0, coord.y );
		vertex.coords = unify::TexCoords( (1.0f / segments) * v, 1 );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (2 * verticesPerSide) + (v * 2) + 1, vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Method 4 - Triangle Strip (Inside)
		// Top
		vertex.pos = unify::V3< float >( coord.x * inner, height, coord.y * inner );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( coord.x, 0, coord.y );
		vertex.coords = unify::TexCoords( (1.0f / segments) * v, 0 );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (3 * verticesPerSide) + (v * 2), vFormat, &vertex );
		vb.GetBBox() += vertex.pos;

		// Bottom
		vertex.pos = unify::V3< float >( coord.x * inner, -height, coord.y * inner );
		vertex.pos += center;
		vertex.normal = unify::V3< float >( coord.x, 0, coord.y );
		vertex.coords = unify::TexCoords( (1.0f / segments) * v, 1 );
		vertex.diffuse = diffuse;
		vertex.specular = specular;
		vd->WriteVertex( lock, (3 * verticesPerSide) + (v * 2) + 1, vFormat, &vertex );
		vb.GetBBox() += vertex.pos;
	}

	vb.Create( vertexCount, vd, vertices.get(), bufferUsage );
}

void shapes::CreateShape_Plane( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
{
	unify::Size< float > size = parameters.Get( "size2", unify::Size< float >( 1.0f, 1.0f ) );
	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
	unsigned int segments = parameters.Get< unsigned int >( "segments", 1 );
	unify::V3< float > center = parameters.Get( "center", unify::V3< float >( 0, 0, 0 ) );
	unify::TexArea texArea = parameters.Get( "texarea", unify::TexArea( unify::TexCoords( 0, 0 ), unify::TexCoords( 1, 1 ) ) );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
	BufferUsage::TYPE bufferUsage = BufferUsage::FromString( parameters.Get( "bufferusage", DefaultBufferUsage ) );
	size_t vertexCount = (segments + 1) * (segments + 1);
	size_t indexCount = 6 * segments * segments;

	BufferSet & set = primitiveList.AddBufferSet();
	VertexBuffer & vb = set.GetVertexBuffer();

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );
	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

	unify::V3< float > posUL = center - unify::V3< float >( size.width * 0.5f, 0, size.height * 0.5f );
	for( unsigned int v = 0; v < (segments + 1); ++v )
	{
		float factorY = 1.0f / float( segments ) * v;
		for( unsigned int h = 0; h < (segments + 1); ++h )
		{
			float factorX = 1.0f / float( segments ) * h;
			unify::V3< float > pos = posUL + unify::V3< float >( size.width * factorX, 0, size.height * factorY );

			unsigned int index = v * (segments + 1) + h;
			vd->WriteVertex( lock, index, positionE, pos );
			vd->WriteVertex( lock, index, normalE, unify::V3< float >( 0, 1, 0 ) );
			vd->WriteVertex( lock, index, diffuseE, diffuse );
			vd->WriteVertex( lock, index, specularE, specular );
			vd->WriteVertex( lock, index, texE, unify::TexCoords( factorX, factorY ) );
			vb.GetBBox() += pos;
		}
		unify::V3< float > pos = center - unify::V3< float >( size.width * 0.5f, 0, size.height * 0.5f );
	}
	vb.Create( vertexCount, vd, vertices.get(), bufferUsage );

	std::vector< Index32 > indices( 6 * segments * segments );
	for( unsigned int v = 0; v < segments; ++v )
	{
		for( unsigned int h = 0; h < segments; ++h )
		{
			Index32 offset = 6 * (h + (v * segments));
			indices[offset + 0] = 0 + (segments + 1) * v + h;
			indices[offset + 1] = 1 + (segments + 1) * v + h;
			indices[offset + 2] = 0 + (segments + 1) * (v + 1) + h;
			indices[offset + 3] = 0 + (segments + 1) * (v + 1) + h;
			indices[offset + 4] = 1 + (segments + 1) * v + h;
			indices[offset + 5] = 1 + (segments + 1) * (v + 1) + h;
		}
	}

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( indexCount, (Index32*)&indices[0], bufferUsage );
}

// A cone...
void shapes::CreateShape_Cone( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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
	VertexBuffer & vb = set.GetVertexBuffer();
	IndexBuffer & ib = set.GetIndexBuffer();

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );
	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

	std::vector< Index32 > indices( indexCount );

	RenderMethodBuffer & rb = set.GetRenderMethodBuffer();

	// Method 1 - Triangle Strip (sides)
	rb.AddMethod( RenderMethod::CreateTriangleStrip( 0, segments * 2, effect ) );

	if( caps )
	{
		rb.AddMethod( RenderMethod::CreateTriangleListIndexed( segments + 1, segments * 3, 0, 0, effect ) );
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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

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
		vd->WriteVertex( lock, (s * 2) + 0, positionE, center );
		vd->WriteVertex( lock, (s * 2) + 0, normalE, norm );
		vd->WriteVertex( lock, (s * 2) + 0, texE, unify::TexCoords( cChange.u * s, texArea.dr.v ) );
		vd->WriteVertex( lock, (s * 2) + 0, diffuseE, diffuse );
		vd->WriteVertex( lock, (s * 2) + 0, specularE, specular );
		vb.GetBBox() += center;


		pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
		norm = pos;
		norm.Normalize();
		vd->WriteVertex( lock, (s * 2) + 1, positionE, pos + center );
		vd->WriteVertex( lock, (s * 2) + 1, normalE, norm );
		vd->WriteVertex( lock, (s * 2) + 1, texE, unify::TexCoords( cChange.u * s, texArea.ul.v ) );
		vd->WriteVertex( lock, (s * 2) + 1, diffuseE, diffuse );
		vd->WriteVertex( lock, (s * 2) + 1, specularE, specular );
		vb.GetBBox() += pos + center;

		if( caps )
		{
			pos = unify::V3< float >( sin( rad ) * radius, height, cos( rad ) * radius );
			norm = pos;
			norm.Normalize();
			vd->WriteVertex( lock, (segments * 2 + 2) + s, positionE, pos + center );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, normalE, norm );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, texE, unify::TexCoords( 0.5f + (float)(sin( rad ) * 0.5f), 0.5f + (float)(cos( rad ) * -0.5f) ) );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, diffuseE, diffuse );
			vd->WriteVertex( lock, (segments * 2 + 2) + s, specularE, specular );
			vb.GetBBox() += pos + center;
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
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, positionE, pos + center );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, normalE, norm );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, texE, unify::TexCoords( 0.5f, 0.5f ) );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, diffuseE, diffuse );
		vd->WriteVertex( lock, segments * 2 + 2 + segments + 1, specularE, specular );
		vb.GetBBox() += pos + center;
	}

	vb.Create( vertexCount, vd, vertices.get(), bufferUsage );

	if( indexCount > 0 )
	{
		ib.Create( indexCount, &indices[0], bufferUsage );
	}
}

void shapes::CreateShape_BeveledBox( core::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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

	unify::Color diffuse = parameters.Get( "diffuse", unify::Color::ColorWhite() );
	unify::Color specular = parameters.Get( "specular", unify::Color::ColorWhite() );
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
	VertexBuffer & vb = set.GetVertexBuffer();

	RenderMethodBuffer & rb = primitiveList.GetBufferSet( 0 ).GetRenderMethodBuffer();
	rb.AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 0, totalVertices, 0, totalTriangles, effect, true ) );

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
	dxi::VertexDeclaration vFormat( renderer, jsonFormat );

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
		vb.GetBBox() += vertices[i].pos;
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


	// Set the vertices from the TEMP vertices...
	std::shared_ptr< unsigned char > verticesRaw( new unsigned char[vd->GetSize() * vertexCount] );
	unify::DataLock lock( verticesRaw.get(), vd->GetSize(), vertexCount, false );

	vd->WriteVertex( lock, { 0, 14, 17 }, vFormat, &vertices[0] );
	vd->WriteVertex( lock, { 1, 15, 20 }, vFormat, &vertices[1] );
	vd->WriteVertex( lock, { 2, 4, 19 }, vFormat, &vertices[2] );
	vd->WriteVertex( lock, { 3, 5, 22 }, vFormat, &vertices[3] );
	vd->WriteVertex( lock, { 6, 8, 18 }, vFormat, &vertices[4] );
	vd->WriteVertex( lock, { 7, 9, 23 }, vFormat, &vertices[5] );
	vd->WriteVertex( lock, { 10, 12, 16 }, vFormat, &vertices[6] );
	vd->WriteVertex( lock, { 11, 13, 21 }, vFormat, &vertices[7] );

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
				vd->WriteVertex( lock, h + (v * 4), texE, vertices[h].coords );
			}
		}

		vd->WriteVertex( lock, 16, texE, unify::TexCoords( 0, 0 ) );
		vd->WriteVertex( lock, 17, texE, unify::TexCoords( 0, 1 ) );
		vd->WriteVertex( lock, 18, texE, unify::TexCoords( 1, 0 ) );
		vd->WriteVertex( lock, 19, texE, unify::TexCoords( 1, 1 ) );

		vd->WriteVertex( lock, 20, texE, unify::TexCoords( 0, 0 ) );
		vd->WriteVertex( lock, 21, texE, unify::TexCoords( 0, 1 ) );
		vd->WriteVertex( lock, 22, texE, unify::TexCoords( 1, 0 ) );
		vd->WriteVertex( lock, 23, texE, unify::TexCoords( 1, 1 ) );
	} break;

	case TextureMode::Wrapped:
	{
		float l1 = 0, l2 = 0.33f;
		float m1 = 0.33f, m2 = 0.66f;
		float r1 = 0.66f, r2 = 1;

		float t1 = 0, t2 = 0.5f;
		float b1 = 0.5f, b2 = 1;

		// Left Side..
		vd->WriteVertex( lock, 12, texE, unify::TexCoords( l1, b1 ) );
		vd->WriteVertex( lock, 13, texE, unify::TexCoords( l1, b2 ) );
		vd->WriteVertex( lock, 14, texE, unify::TexCoords( l2, b1 ) );
		vd->WriteVertex( lock, 15, texE, unify::TexCoords( l2, b2 ) );

		// Front...
		vd->WriteVertex( lock, 0, texE, unify::TexCoords( m1, b1 ) );
		vd->WriteVertex( lock, 1, texE, unify::TexCoords( m1, b2 ) );
		vd->WriteVertex( lock, 2, texE, unify::TexCoords( m2, b1 ) );
		vd->WriteVertex( lock, 3, texE, unify::TexCoords( m2, b2 ) );

		// Right Side...
		vd->WriteVertex( lock, 4, texE, unify::TexCoords( r1, b1 ) );
		vd->WriteVertex( lock, 5, texE, unify::TexCoords( r1, b2 ) );
		vd->WriteVertex( lock, 6, texE, unify::TexCoords( r2, b1 ) );
		vd->WriteVertex( lock, 7, texE, unify::TexCoords( r2, b2 ) );

		// Rear...
		vd->WriteVertex( lock, 8, texE, unify::TexCoords( l1, t1 ) );
		vd->WriteVertex( lock, 9, texE, unify::TexCoords( l1, t2 ) );
		vd->WriteVertex( lock, 10, texE, unify::TexCoords( l2, t1 ) );
		vd->WriteVertex( lock, 11, texE, unify::TexCoords( l2, t2 ) );

		// Top...
		vd->WriteVertex( lock, 16, texE, unify::TexCoords( m1, t1 ) );
		vd->WriteVertex( lock, 17, texE, unify::TexCoords( m1, t2 ) );
		vd->WriteVertex( lock, 18, texE, unify::TexCoords( m2, t1 ) );
		vd->WriteVertex( lock, 19, texE, unify::TexCoords( m2, t2 ) );

		// Bottom...
		vd->WriteVertex( lock, 20, texE, unify::TexCoords( r1, t1 ) );
		vd->WriteVertex( lock, 21, texE, unify::TexCoords( r1, t2 ) );
		vd->WriteVertex( lock, 22, texE, unify::TexCoords( r2, t1 ) );
		vd->WriteVertex( lock, 23, texE, unify::TexCoords( r2, t2 ) );
	} break;
	}

	// Allow per-face diffuse...
	if( diffuses.size() == 6 )
	{
		// Front...
		vd->WriteVertex( lock, { 0, 1, 2, 3 }, diffuseE, diffuses[1] );

		// Right Side...
		vd->WriteVertex( lock, { 4, 5, 6, 7 }, diffuseE, diffuses[2] );

		// Rear...
		vd->WriteVertex( lock, { 8, 9, 10, 11 }, diffuseE, diffuses[3] );

		// Left Side..
		vd->WriteVertex( lock, { 12, 13, 14, 15 }, diffuseE, diffuses[0] );

		// Top...
		vd->WriteVertex( lock, { 16, 17, 18, 18 }, diffuseE, diffuses[4] );

		// Bottom...
		vd->WriteVertex( lock, { 20, 21, 22, 23 }, diffuseE, diffuses[5] );
	}

	vb.Create( totalVertices, vd, verticesRaw.get(), bufferUsage );

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

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( totalIndices, indices, bufferUsage );
}