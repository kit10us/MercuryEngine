// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <sg/ShapeCreators.h>
#include <me/RenderMethod.h>
#include <me/TextureMode.h>
#include <me/exception/NotImplemented.h>
#include <me/BufferUsage.h>
#include <me/VertexUtil.h>
#include <unify/String.h>
#include <unify/Size3.h>
#include <unify/TexArea.h>
#include <unify/Angle.h>

using namespace me;

const float PI = 3.14159265358979f;
const float PI2 = 6.28318530717959f;
const std::string DefaultBufferUsage = "Default";

void sg::CreateShape_Sphere( me::IRenderer * renderer, PrimitiveList & primitiveList, unify::Parameters & parameters )
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

		size_t vertexCount = (iFacesH + 1) * (iFacesV + 1);

		int iNumFaces = iFacesH * iFacesV * 2;	// Twice as many to count for triangles
		unsigned int indexCount = iNumFaces * 3;			// Three indices to a triangle

		BufferSet & set = primitiveList.AddBufferSet();

		// Method 1 - Triangle List...
		set.AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

		std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSizeInBytes( 0 ) * vertexCount] );
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
				WriteVertex( *vd, lock, iVert, positionE, vec );
				WriteVertex( *vd, lock, iVert, normalE, norm );
				WriteVertex( *vd, lock, iVert, diffuseE, diffuse );
				WriteVertex( *vd, lock, iVert, specularE, specular );
				WriteVertex( *vd, lock, iVert, texE, unify::TexCoords( h * (1.0f / iFacesH), v * (1.0f / iFacesV) ) );
				vbParameters.bbox += vec;
				iVert++;
			}
		}

		set.AddVertexBuffer( vbParameters );

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

		set.AddIndexBuffer( { { { indexCount, (Index32*)&indices[0] } }, bufferUsage } );
	}
	else
	{	// STRIP VERSION
		int iRows = (int)segments;
		int iColumns = (int)iRows / 2;
		size_t vertexCount = (iRows + 1) * (iColumns + 1);
		unsigned int indexCount = (iColumns * (2 * (iRows + 1))) + (((iColumns - 1) * 2));

		BufferSet & set = primitiveList.AddBufferSet();

		// Method 1 - Triangle Strip...
		RenderMethod renderMethod( RenderMethod::CreateTriangleStripIndexed( vertexCount, indexCount, 0, 0, effect ) );
		set.AddMethod( renderMethod );

		std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSizeInBytes( 0 ) * vertexCount] );

		unsigned short stream = 0;

		VertexElement positionE = CommonVertexElement::Position( stream );
		VertexElement normalE = CommonVertexElement::Normal( stream );
		VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
		VertexElement specularE = CommonVertexElement::Specular( stream );
		VertexElement texE = CommonVertexElement::TexCoords( stream );
								
		unify::V3< float > vec, norm;
		unify::TexCoords coords;

		// Set the vertices...
		unify::DataLock lock( vertices.get(), vd->GetSizeInBytes( 0 ), vertexCount, false, 0 );

		VertexBufferParameters vbParameters{ vd, { { vertexCount, vertices.get() } }, bufferUsage };

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

				WriteVertex( *vd, lock, iVert, positionE, vec );
				WriteVertex( *vd, lock, iVert, normalE, norm );
				WriteVertex( *vd, lock, iVert, diffuseE, diffuse );
				WriteVertex( *vd, lock, iVert, specularE, specular );
				WriteVertex( *vd, lock, iVert, texE, coords );
				vbParameters.bbox += vec;
				iVert++;
			}
		}
		
		set.AddVertexBuffer( vbParameters );

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

		set.AddIndexBuffer( { { { indexCount, &indices[0] } }, bufferUsage } );
	}
}
