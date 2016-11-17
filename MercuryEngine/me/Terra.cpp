// Copyright (c) 2002 - 2010 Quentin S. Smith
// All Rights Reserved
 
#include <me/Terra.h>
#include <unify/Math.h>
#include <me/VertexUtil.h>

using namespace me;

Terra::Terra( me::IRenderer * renderer )
: Mesh( renderer )
, m_depth{}
, m_pointCount( 0, 0 )
{
}

Terra::Terra( me::IRenderer * renderer, unify::Parameters & parameters )
: Terra( renderer )
{
    CreateFromParameters( parameters );
}

Terra::~Terra()
{
	Destroy();
}

void Terra::Destroy()
{
	delete[] m_depth;
	m_depth = 0;
}

void Terra::CreateFromParameters( unify::Parameters & parameters )
{
	Destroy();

	unify::RowColumn< unsigned int > faces;
	if ( parameters.Exists( "faces" ) )
	{
		faces = parameters.Get< unify::RowColumn< unsigned int > >( "faces" );
		m_pointCount = faces + unify::RowColumn< unsigned int >( 1, 1 );
	}
	else if ( parameters.Exists( "points" ) )
	{
		m_pointCount = parameters.Get< unify::RowColumn< unsigned int > >( "points" );
		faces = m_pointCount - unify::RowColumn< unsigned int >( 1, 1 );
	}
	else
	{
		throw unify::Exception( "Terra requires either faces or points!" );
	}

    m_size = parameters.Get< unify::Size< float > > ( "size" );
    const unify::TexArea texArea = parameters.Get< unify::TexArea >( "texarea", unify::TexAreaFull() );
	const float constant = parameters.Get< float >( "constant" );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );

	unify::ColorUnit diffuseUL = parameters.Get< unify::ColorUnit >( "diffuseul", unify::ColorUnit::ColorUnitWhite() );
	unify::ColorUnit diffuseUR = parameters.Get< unify::ColorUnit >( "diffuseur", unify::ColorUnit::ColorUnitWhite() );
	unify::ColorUnit diffuseDL = parameters.Get< unify::ColorUnit >( "diffusedl", unify::ColorUnit::ColorUnitWhite() );
	unify::ColorUnit diffuseDR = parameters.Get< unify::ColorUnit >( "diffusedr", unify::ColorUnit::ColorUnitWhite() );

	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();
		  
	const size_t && IndicesPerTriangle = 3;
	const size_t && TrianglesPerCell = 2;
	const size_t && IndicesPerCell = IndicesPerTriangle * TrianglesPerCell;
	const size_t && IndicesPerRow = IndicesPerCell * faces.row;

	unsigned int vertexCount = (faces.row + 1 ) * (faces.column + 1);
	//int indexCount = (faces.row * (2 * (faces.row + 1))) + (((faces.column - 1) * 2));
	unsigned int indexCount = faces.CellCount() * 3 * 2;

	m_depth = new float[vertexCount];
	m_minmax.Clear();

	BufferSet & set = GetPrimitiveList().AddBufferSet();

	set.GetRenderMethodBuffer().AddMethod( RenderMethod::CreateTriangleListIndexed( vertexCount, indexCount, 0, 0, effect ) );

	unify::BBox< float > bbox;

	// Fill in vertices & Build depth buffer...
	unsigned short stream = 0;

	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement normalE = CommonVertexElement::Normal( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	VertexElement specularE = CommonVertexElement::Specular( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize( 0 ) * vertexCount] );
	unify::DataLock lock( vertices.get(), vd->GetSize( 0 ), vertexCount, false, 0 );

	// Build depth buffer...	
	bool hasHeightHap = false;
	if ( parameters.Exists( "heightmap" ) )
	{
		TextureOpMap heightMap = parameters.Get< TextureOpMap >( "heightmap" );
		unify::Size< unsigned int > heightMapSize = heightMap.texture->ImageSize();
		unify::TexCoords uvHeight_Length = unify::TexCoords( heightMap.texArea.Size().width, heightMap.texArea.Size().height );
		unify::TexCoords height_uv;

		TextureLock textlock;
		heightMap.texture->LockRect( 0, textlock, 0, true );

		for ( unsigned int c = 0; c < (faces.column + 1); c++ )
		{
			for ( unsigned int r = 0; r < (faces.row + 1); r++ )
			{
				// Compute texcoord in image...
				unify::TexCoords uv( 
					(float)r / ((float)faces.row + 1), 
					(float)c / ((float)faces.column + 1) 
				);

				// Get the heightmap position...
				height_uv = heightMap.texArea.ul + (uv * uvHeight_Length);

				// Support tiling...
				while ( height_uv.v > 1.0f )
				{
					height_uv.v -= 1.0f;
				}
				while ( height_uv.v < 0.0f )
				{
					height_uv.v += 1.0f;
				}

				while ( height_uv.u > 1.0f )
				{
					height_uv.u -= 1.0f;
				}
				while ( height_uv.u < 0.0f )
				{
					height_uv.u += 1.0f;
				}

				/*
				unsigned int uSurfaceOffsetV = (unsigned int)(heightMapSize.height * height_uv.v);

				size_t offset = (uSurfaceOffsetV * textlock.uStride);
				unsigned char* pBufferColumnStart = (unsigned char*)((unsigned char*)textlock.pBits + (uSurfaceOffsetV * textlock.uStride));

				unsigned int uSurfaceOffsetH = (unsigned int)(heightMapSize.width * height_uv.u);
				unsigned char* pBuffer = pBufferColumnStart + uSurfaceOffsetH;
				unify::Color pixel = unify::Color::ColorARGB( *pBuffer );
				unify::ColorUnit result( heightMap.colorOp * pixel );
				*/

				unsigned int v = (unsigned int)(heightMapSize.height * height_uv.v);
				unsigned int h = (unsigned int)(heightMapSize.width * height_uv.u);
				unify::Color pixel = *(unify::Color*)
					(
					((unsigned char*)textlock.pBits) + ( v * textlock.uStride + (h * textlock.bpp))
						);

				unify::ColorUnit result( heightMap.colorOp * pixel );


				// Perform modification to vertex...
				float sum = result.SumComponents();

				unsigned int vertexIndex = r + (c * (faces.row + 1));
				float depth = constant + sum;
				m_depth[vertexIndex] = depth;
				m_minmax += depth;

			}
		}
		heightMap.texture->UnlockRect( 0 );
	}

	unsigned int uVert = 0;

	unify::TexCoords uvLength;
	unify::TexCoords uv;
	
	uvLength = texArea.dr - texArea.ul;

	for ( unsigned int c = 0; c < (faces.column + 1); c++ )
	{
		float v = (1.0f / (faces.column)) * c; // is unit
		uv.v = (texArea.ul.v + ((v) * uvLength.v));

		for ( unsigned int r = 0; r < (faces.row + 1); r++ )
		{
			float h = (1.0f / (faces.row)) * r;	// is unit
			uv.u = texArea.ul.u + (uvLength.u * h);

			unsigned int vertex = (c * (faces.row + 1)) + r;

			unify::V3< float > vPos(
				(m_size.width * -0.5f) + m_size.width * h
				, m_depth[vertex]
				, (m_size.height * 0.5f) - m_size.height * v
			);

			unify::V3< float > vNormal( 0, 1, 0 );

			unify::ColorUnit ca( unify::Lerp( diffuseUL, diffuseUR, h ) );
			unify::ColorUnit cb( unify::Lerp( diffuseDL, diffuseDR, v ) );
			unify::ColorUnit cc( (ca + cb) / 2.0f );
									   
			unify::Color diffuse( cc );
									   
			WriteVertex( *vd, lock, vertex, positionE, vPos );
			WriteVertex( *vd, lock, vertex, normalE, vNormal );
			WriteVertex( *vd, lock, vertex, texE, uv );
			WriteVertex( *vd, lock, vertex, diffuseE, diffuse );
			uVert++;
		}
	}

	set.AddVertexBuffer( { vertexCount, vd, 0, vertices.get(), BufferUsage::Default, bbox } );

	// Fill in indices...
	std::vector< Index32 > indices( indexCount );

	for ( unsigned int c = 0; c < faces.column; ++c )
	{
		for ( unsigned int r = 0; r < faces.row; ++r )
		{
			// Build quad
			unsigned int v0 = c * (faces.row + 1) + r;
			unsigned int v1 = v0 + 1;
			unsigned int v2 = v0 + (faces.row + 1 );
			unsigned int v3 = v2 + 1;

			unsigned int firstIndex = IndicesPerRow * c + r * IndicesPerCell;
			indices[firstIndex + 0] = v0;
			indices[firstIndex + 1] = v1;
			indices[firstIndex + 2] = v2;
			indices[firstIndex + 3] = v1;
			indices[firstIndex + 4] = v3;
			indices[firstIndex + 5] = v2; 
		}
	}

	set.AddIndexBuffer( { indexCount, (Index32*)&indices[0], BufferUsage::Default } );
				 
	ComputeBounds();
}

bool Terra::ApplyHeightMap( TextureOpMap tom )
{
	assert( 0 );
	/*
	// Fill in vertices & Build depth buffer...

	TextureLock textlock;
	tom.texture->LockRect( 0, textlock, 0, true );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();
	VertexElement diffuseE = CommonVertexElement::Diffuse();

	unsigned int* pBuffer;
	unsigned int* pBufferColumnStart;

	unify::TexCoords uvHeight_Length;
	unify::TexCoords height_uv;
	uvHeight_Length = unify::TexCoords( tom.texArea.Size().width, tom.texArea.Size().height );

	unify::Size< unsigned int > imageSize = tom.texture->ImageSize();
	for( unsigned int vert = 0; vert < lock.Count(); ++vert )
	{
		unify::V3< float > vPos;
		ReadVertex( *vd, lock, vert, positionE, vPos );

		unify::V3< float > barry = GetBBox().ToBarrycentric( vPos );
		barry.z = 1.0f - barry.z; // Reverse z so we match the orientation of the image.

								  // Get the heightmap position...
		height_uv.v = tom.texArea.ul.v + (barry.z * uvHeight_Length.v);
		while( height_uv.v > 1.0f )
		{
			height_uv.v -= 1.0f;
		}
		while( height_uv.v < 0.0f )
		{
			height_uv.v += 1.0f;
		}

		height_uv.u = tom.texArea.ul.u + (barry.x * uvHeight_Length.u);
		while( height_uv.u > 1.0f )
		{
			height_uv.u -= 1.0f;
		}
		while( height_uv.u < 0.0f )
		{
			height_uv.u += 1.0f;
		}

		unsigned int uSurfaceOffsetV = (unsigned int)(imageSize.height * height_uv.v);
		pBufferColumnStart = (unsigned int*)((unsigned int*)textlock.pBits + (uSurfaceOffsetV * textlock.uStride));

		unsigned int uSurfaceOffsetH = (unsigned int)(imageSize.width * height_uv.u);
		pBuffer = pBufferColumnStart + uSurfaceOffsetH;
		unify::Color pixel = unify::Color::ColorARGB( *pBuffer );
		unify::ColorUnit result( tom.colorOp * pixel );

		// Perform modification to vertex...
		float sum = result.SumComponents();
		vPos.y += sum;

		// Update vertex in buffer...
		WriteVertex( *vd, lock, vert, positionE, vPos );
	}
	tom.texture->UnlockRect( 0 );

	set.GetVertexBuffer().Unlock();

	ComputeBounds();
	*/

	return true;
}


bool Terra::ApplyAlphaMap( TextureOpMap tom )
{
	assert( 0 );

	/*
	// Fill in vertices & Build depth buffer...

	TextureLock textlock;
	tom.texture->LockRect( 0, textlock, 0, true );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();
	VertexElement diffuseE = CommonVertexElement::Diffuse();

	unsigned int* pBuffer;
	unsigned int* pBufferColumnStart;

	unify::TexCoords uvHeight_Length;
	unify::TexCoords height_uv;
	uvHeight_Length = unify::TexCoords( tom.texArea.Size().width, tom.texArea.Size().height );

	unify::Size< unsigned int > imageSize = tom.texture->ImageSize();
	for( unsigned int vert = 0; vert < lock.Count(); ++vert )
	{
		unify::V3< float > vPos;
		ReadVertex( *vd, lock, vert, positionE, vPos );

		unify::V3< float > barry = GetBBox().ToBarrycentric( vPos );
		barry.z = 1.0f - barry.z; // Reverse z so we match the orientation of the image.

								  // Get the heightmap position...
		height_uv.v = tom.texArea.ul.v + (barry.z * uvHeight_Length.v);
		while( height_uv.v > 1.0f )
		{
			height_uv.v -= 1.0f;
		}
		while( height_uv.v < 0.0f )
		{
			height_uv.v += 1.0f;
		}

		height_uv.u = tom.texArea.ul.u + (barry.x * uvHeight_Length.u);
		while( height_uv.u > 1.0f )
		{
			height_uv.u -= 1.0f;
		}
		while( height_uv.u < 0.0f )
		{
			height_uv.u += 1.0f;
		}

		unsigned int uSurfaceOffsetV = (unsigned int)(imageSize.height * height_uv.v);
		pBufferColumnStart = (unsigned int*)((unsigned int*)textlock.pBits + (uSurfaceOffsetV * textlock.uStride));

		unsigned int uSurfaceOffsetH = (unsigned int)(imageSize.width * height_uv.u);
		pBuffer = pBufferColumnStart + uSurfaceOffsetH;
		unify::Color pixel = unify::Color::ColorARGB( *pBuffer );
		unify::ColorUnit result( tom.colorOp * pixel );

		// Perform modification to vertex...
		unify::Color colDiffuse;
		ReadVertex( *vd, lock, vert, diffuseE, colDiffuse );
		unify::ColorUnit colUnit;
		colUnit = colDiffuse;

		colUnit.a = result.SumComponents();
		colDiffuse = colUnit;

		// Update vertex in buffer...
		WriteVertex( *vd, lock, vert, diffuseE, colDiffuse );

	}
	tom.texture->UnlockRect( 0 );

	set.GetVertexBuffer().Unlock();

	ComputeBounds();
	*/

	return true;
}


bool Terra::ApplyTextureMap( unsigned int dwMember, const unify::TexArea * pTexArea )
{
	assert( 0 );

	/*
	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexElement positionE = CommonVertexElement::Position();

	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	unify::TexCoords cMul( pTexArea->Size().width, pTexArea->Size().height );
	cMul.u /= (rc.row - 1);
	cMul.v /= (rc.column - 1);

	unsigned int r, c;
	unsigned int uVert = 0;
	for( c = 0; c < rc.column; c++ )
	{
		for( r = 0; r < rc.row; r++ )
		{
			// TODO: Not yet supported.
			//vertWriteVertex( *vd, lock, uVert, &unify::TexCoords( pTexArea->ul.u + (cMul.u * r), pTexArea->dr.v - (cMul.v * c) ), (FVF::TYPE)dwMember );
			uVert++;
		}
	}
	set.GetVertexBuffer().Unlock();
	*/
	return true;		
}

bool Terra::Smooth( unsigned int uFlags )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	float * pfDepthArray = new float[ rc.column * rc.row ];
	if( ! pfDepthArray )
	{
		return false;
	}

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();
	VertexElement positionE = CommonVertexElement::Position();	
	
	// Compute new smoothed depth...
	unsigned int r, c;
	for( c = 0; c < rc.column; c++ )
	{
		for( r = 0; r < rc.row; r++ )
		{
			float fParts = 0;
			float fDepth = 0.0f;

			unify::V3< float > vVecIn;

			// Left vertex...
			if( r > 0 )
			{
				ReadVertex( *vd, lock, (r-1) + (c * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Right vertex...
			if( r < (rc.row-1) )
			{
				ReadVertex( *vd, lock, (r+1) + (c * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Up vertex...
			if( c > 0 )
			{
				ReadVertex( *vd, lock, (r) + ((c-1) * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Down vertex...
			if( c < (rc.column-1) )
			{
				ReadVertex( *vd, lock, (r) + ((c+1) * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}

			// Assign present vertex...
			ReadVertex( *vd, lock, (r + c * rc.row), positionE, vVecIn );

			fDepth += vVecIn.y * 2;
			fParts += 2;

			pfDepthArray[r + (c * rc.row)] = fDepth / fParts;
		}
	}

	// Assign new depth...
	for( c = 0; c < rc.column; c++ )
	{
		for( r = 0; r < rc.row; r++ )
		{
			// Assign present vertex...
			unify::V3< float > vec;
			ReadVertex( *vd, lock, r + c * rc.row, positionE, vec );
			vec.y = pfDepthArray[r + (c * rc.row)];
			WriteVertex( *vd, lock, r + c * rc.row, positionE, vec );
		}
	}

	delete [] pfDepthArray;
	pfDepthArray = 0;

	set.GetVertexBuffer().Unlock();
	*/
	return true;
}

bool Terra::ApplyTransparent( unsigned int uFlags, float fValue, float fTolerance )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();
	VertexElement diffuseE = CommonVertexElement::Diffuse();

	// ...
	unsigned int r, c;
	for( c = 0; c < rc.column; c++ )
	{
		for( r = 0; r < rc.row; r++ )
		{
			unify::V3< float > vVecIn;
			float fDepth = 0.0f;
			ReadVertex( *vd, lock, r + c * rc.row, positionE, vVecIn );
			fDepth = vVecIn.y;

			if( unify::CheckFlag( uFlags, TRANS_BELOW ) && fDepth < fValue )
			{
				unify::Color colorIn;
				ReadVertex( *vd, lock, r + c * rc.row, diffuseE, colorIn );
				unify::ColorUnit colorMix = colorIn;
				colorMix.a = 0.0f;
				colorIn = colorMix;
				WriteVertex( *vd, lock, r + c * rc.row, diffuseE, colorIn );
			}
			else if( unify::CheckFlag( uFlags, TRANS_ABOVE ) && fDepth > fValue )
			{
				unify::Color colorIn;
				ReadVertex( *vd, lock, r + c * rc.row, diffuseE, colorIn );
				unify::ColorUnit colorMix = colorIn;
				colorMix.a = 0.0f;
				colorIn = colorMix;
				WriteVertex( *vd, lock, r + c * rc.row, diffuseE, colorIn );
			}
		}
	}

	set.GetVertexBuffer().Unlock();
	*/

	return true;
}

// Fix either ROWS or COLUMNS to the average of both sides.
bool Terra::MakeWrappable( unsigned int uFlags )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();

	// Compute new wrapped depths...
	unsigned int r, c;
	float fDepth;
	
	// Rows...
	if( unify::CheckFlag(uFlags,WRAP_ROWS) )
	{
		for( r = 0; r < rc.row; r++ )
		{
			unify::V3< float > vec1, vec2;
			ReadVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec1 );
			ReadVertex( *vd, lock, (r) + ((rc.column - 1) * rc.row), positionE, vec2 );

			fDepth = (vec1.y + vec2.y) / 2.0f;
			vec1.y = fDepth;
			vec2.y = fDepth;

			WriteVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec1 );
			WriteVertex( *vd, lock, (r) + ((rc.column - 1) * rc.row), positionE, vec2 );
		}
	}

	// Columns...
	if( unify::CheckFlag(uFlags,WRAP_COLUMNS) )
	{
		for( c = 0; c < rc.column; c++ )
		{
			unify::V3< float > vec1, vec2;
			ReadVertex( *vd, lock, (0) + (c * rc.row), positionE, vec1 );
			ReadVertex( *vd, lock, (rc.row - 1) + (c * rc.row), positionE, vec2 );
			
			fDepth = (vec1.y + vec2.y) / 2.0f;
			vec1.y = fDepth;
			vec2.y = fDepth;

			WriteVertex( *vd, lock, (0) + (c * rc.row), positionE, vec1);
			WriteVertex( *vd, lock, (rc.row - 1) + (c * rc.row), positionE, vec2 );
		}
	}

	// Corners...
	if( unify::CheckFlag(uFlags,WRAP_ROWS) && unify::CheckFlag(uFlags,WRAP_COLUMNS) )
	{
		unify::V3< float > vec[4];
		ReadVertex( *vd, lock, (0) + (0 * rc.row), positionE, vec[0] );
		ReadVertex( *vd, lock, (rc.row - 1) + (0 * rc.row), positionE, vec[1] );
		ReadVertex( *vd, lock, (0) + ((rc.column - 1) * rc.row), positionE, vec[2] );
		ReadVertex( *vd, lock, (rc.row - 1) + ((rc.column - 1) * rc.row), positionE, vec[3] );

		fDepth = (vec[0].y + vec[1].y + vec[2].y + vec[3].y) / 4.0f;

		vec[0].y = fDepth;
		vec[1].y = fDepth;
		vec[2].y = fDepth;
		vec[3].y = fDepth;

		WriteVertex( *vd, lock, (rc.row - 1) + (0 * rc.row), positionE, vec[1] );
		WriteVertex( *vd, lock, (0) + ((rc.column - 1) * rc.row), positionE, vec[2] );
		WriteVertex( *vd, lock, (rc.row - 1) + ((rc.column - 1) * rc.row), positionE, vec[3] );
	}

	set.GetVertexBuffer().Unlock();
	*/
	
	return true;
}

// Set a column or row to a given depth value.
bool Terra::FixSide( unsigned int uFlags, float fToDepth )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row , m_pointCount.column );
	
	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();

	// Compute new wrapped depths...
	unsigned int r, c;
	
	unify::V3< float > vec;

	// Rows...
	for( r = 0; r < rc.row; r++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_DOWN) )
		{
			ReadVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec );
			vec.y = fToDepth;
			WriteVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			ReadVertex( *vd, lock, (r) + ((rc.column - 1) * rc.row), positionE, vec );
			vec.y = fToDepth;
			WriteVertex( *vd, lock, (r) + ((rc.column - 1) * rc.row), positionE, vec );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			ReadVertex( *vd, lock, (0) + (c * rc.row), positionE, vec );
			vec.y = fToDepth;
			WriteVertex( *vd, lock, (0) + (c * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			ReadVertex( *vd, lock, (rc.row - 1) + (c * rc.row), positionE, vec );
			vec.y = fToDepth;
			WriteVertex( *vd, lock, (rc.row - 1) + (c * rc.row), positionE, vec );
		}
	}

	set.GetVertexBuffer( ).Unlock();
	*/
	return true;
}

// Align a side to a given terra's side
bool Terra::AlignSide( unsigned int uFlags, Terra * pTerraIn )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();

	unify::DataLock lockIn;
	BufferSet & setIn = pTerraIn->m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	setIn.GetVertexBuffer().Lock( lockIn );

	VertexDeclaration::ptr vdIn = setIn.GetVertexBuffer().GetVertexDeclaration();


	// Compute new wrapped depths...
	unsigned int r, c;

	unify::V3< float > vec, vecin;
	
	// Rows...
	for( r = 0; r < rc.row; r++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_DOWN) )
		{
			ReadVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec );
			ReadVertex( *vdIn, lockIn, (r) + ((rc.column-1) * rc.row), positionE, vecin );
			vec.y = vecin.y;
			WriteVertex( *vd, lock, (r) + (0 * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			ReadVertex( *vd, lock, (r) + ((rc.column-1) * rc.row), positionE, vec );
			ReadVertex( *vdIn, lockIn, (r) + (0 * rc.row), positionE, vecin );
			vec.y = vecin.y;
			WriteVertex( *vd, lock, (r) + ((rc.column-1) * rc.row), positionE, vec );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			ReadVertex( *vd, lock, (0) + (c * rc.row), positionE, vec );
			ReadVertex( *vdIn, lockIn, (rc.row-1) + (c * rc.row), positionE, vecin );
			vec.y = vecin.y;
			WriteVertex( *vd, lock, (0) + (c * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			ReadVertex( *vd, lock, (rc.row-1) + (c * rc.row), positionE, vec );
			ReadVertex( *vdIn, lockIn, (0) + (c * rc.row), positionE, vecin );
			vec.y = vecin.y;
			WriteVertex( *vd, lock, (rc.row-1) + (c * rc.row), positionE, vec );
		}
	}

	pTerraIn->m_primitiveList.GetBufferSet( 0 ).GetVertexBuffer().Unlock();
	set.GetVertexBuffer().Unlock();
	*/
	return true;
}

// Set the normals on the side(s) specified to a given normal
bool Terra::NormalSide( unsigned int uFlags, const unify::V3< float > & normal )
{
	assert( 0 );
	/*
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_pointCount.row, m_pointCount.column );

	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();

	VertexElement positionE = CommonVertexElement::Position();
	VertexElement normalE = CommonVertexElement::Normal();

	// Compute new wrapped depths...
	unsigned int r, c;
	unify::V3< float > vec;
	
	// Rows...
	for( r = 0; r < rc.row; r++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_DOWN) )
		{
			WriteVertex( *vd, lock, (r) + (0 * rc.row), normalE, normal );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			WriteVertex( *vd, lock, (r) + ((rc.column-1) * rc.row), normalE, normal );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			WriteVertex( *vd, lock, (0) + (c * rc.row), normalE, normal );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			WriteVertex( *vd, lock, (rc.row-1) + (c * rc.row), normalE, normal );
		}
	}

	set.GetVertexBuffer().Unlock();
	*/
	return true;
}

// Offset the entire geometry internally (does not effect any matrix of frame position).
void Terra::Offset( const unify::V3< float > & vec )
{
	/*
	m_primitiveList.GetVertexBuffer().Offset( vec );
	ComputeBounds();
	*/
}

bool Terra::RenderNormals()
{

	/* TODO:
	// Render normals...
	VertexBuffer NormalsVB;

	unify::V3< float > vFromPos, vFromNorm;
	
	unsigned int dwNumNormals = m_primitiveList.GetNumVertices();
			
	unify::DataLock lock;
	m_primitiveList.GetVertexBuffer().Lock( lock );

	// Create new normals...
	unsigned int dwNormal, dwVertex;

	// Create the VB for the Normals...
	NormalsVB.Create( dwNumNormals * 2, FVF::XYZ | FVF::Diffuse, BufferUsage::Default );

	struct MyVert { unify::V3< float > p; unify::Color diffuse; };
	MyVert* pVertex;

	float fNormalSize = 1.0f;

	// Lock the vertex buffer
	NormalsVB.Lock( (void**)&pVertex );

	dwNormal = 0;
	for( dwVertex = 0; dwVertex < dwNumNormals; dwVertex++ )
	{
		ReadVertex( *vd, lock, dwVertex, &vFromPos, FVF::XYZ );
		ReadVertex( *vd, lock, dwVertex, &vFromNorm, FVF::Normal );

		pVertex[dwNormal].p = vFromPos;
		pVertex[dwNormal].diffuse = unify::Color::White();
		dwNormal++;

		unify::V3< float > v;

		pVertex[dwNormal].p = vFromPos;
		pVertex[dwNormal].p += vFromNorm * fNormalSize; 
										//unify::V3< float >(	pFromVertex[dwVertex].n.x * fNormalSize,
										//	pFromVertex[dwVertex].n.y * fNormalSize,
										//	pFromVertex[dwVertex].n.z * fNormalSize );
		pVertex[dwNormal].diffuse = unify::Color::White();
		dwNormal++;
	}
	NormalsVB.Unlock();
	m_primitiveList.GetVertexBuffer().Unlock();

	NormalsVB.Render( RenderMethod( PT_LineList, 0, dwNumNormals, 0 ) );

	NormalsVB.Release();
	*/

	/*
	// Render Bounding Box...
	QShape bbox;
	ShapeArgs args;
	args.SetPoints( 2, (unify::V3< float > *)&GetBBox() );
	bbox.CreateShape( Shape::Cube, &args );
	bbox.Render();
	*/
	return true;
}

unify::RowColumn< unsigned int > Terra::GetPointCount() const
{
	return m_pointCount;
}

float Terra::GetDepth( float x, float y ) const
{
	if ( x > 1.0f ) x = 1.0f;
	if ( x < 0.0f ) x = 0.0f;

	if ( y > 1.0f ) y = 1.0f;
	if ( y < 0.0f ) y = 0.0f;

	// Get the vertex row and column.
	unify::RowColumn< unsigned int > rc;
	rc.row = (unsigned int)(((float)m_pointCount.row) * x);
	rc.column = (unsigned int)(((float)m_pointCount.column + 1.0f) * y);
	return GetDepth( rc );
}

float Terra::GetDepth( unify::RowColumn< unsigned int > rc ) const
{
	if ( rc.row >= m_pointCount.row ) rc.row = m_pointCount.row - 1;
	if ( rc.row < 0 ) rc.row = 0;

	if ( rc.column >= m_pointCount.column ) rc.column = m_pointCount.column - 1;
	if ( rc.column < 0 ) rc.column = 0;

	unsigned int vertex = rc.row + rc.column * m_pointCount.row;
	return m_depth[vertex];

}

unify::MinMax< float > Terra::GetMinMax() const
{
	return m_minmax;
}

unify::Size< float > Terra::GetSize() const
{
	return m_size;
}