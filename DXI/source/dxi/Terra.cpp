// Copyright (c) 2002 - 2010 Quentin S. Smith
// All Rights Reserved
 
#include <dxi/Terra.h>

using namespace dxi;

Terra::Terra()
: m_rc( 0, 0 )
{
}

Terra::Terra( unify::Parameters & parameters )
: Terra()
{
    CreateFromParameters( parameters );
}

Terra::~Terra()
{
	Destroy();
}

void Terra::Destroy()
{
    m_depth.clear();
	m_primitiveList.Destroy();
}

const unify::BBox< float > & Terra::ComputeBounds()
{
	m_primitiveList.ComputeBounds( m_BBox );
	return m_BBox;
}

void Terra::CreateFromParameters( unify::Parameters & parameters )
{
	Destroy();

    const unify::RowColumn< unsigned int > rc = parameters.Get< unify::RowColumn< unsigned int > >( "rowscolumns" );
    const unify::Size< float > size = parameters.Get< unify::Size< float > > ( "size" );
    const unify::TexArea texArea = parameters.Get< unify::TexArea >( "texarea" );
	const float constant = parameters.Get< float >( "constant" );
	Effect::ptr effect = parameters.Get< Effect::ptr >( "effect" );
	VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();

	int vertexCount = (rc.row + 1) * (rc.column + 1 );
	int indexCount = (rc.column * (2 * (rc.row + 1))) + (((rc.column - 1) * 2));

	BufferSet & set = m_primitiveList.AddBufferSet();

	std::shared_ptr< unsigned char > vertices( new unsigned char[vd->GetSize() * vertexCount] );

	// Method 1 - Triangle Strip...
	set.GetRenderMethodBuffer().AddMethod( RenderMethod( PrimitiveType::TriangleStrip, 0 /*baseVertexIndex*/, 0 /*minIndex*/, vertexCount, 0 /*startIndex*/, indexCount - 2, effect, true ) );

	// Fill in vertices & Build depth buffer...
	unsigned int r, c;

	unsigned short stream = 0;

	VertexElement positionE = CommonVertexElement::Position( stream );
	VertexElement normalE = CommonVertexElement::Normal( stream );
	VertexElement diffuseE = CommonVertexElement::Diffuse( stream );
	VertexElement specularE = CommonVertexElement::Specular( stream );
	VertexElement texE = CommonVertexElement::TexCoords( stream );

	unify::DataLock lock( vertices.get(), vd->GetSize(), vertexCount, false );

	float fStartX = size.width * -0.5f;
	float fStartY = size.height * -0.5f;
	float fRatioForRows = size.width / rc.row;
	float fRatioForColumns = size.height / rc.column;
	unsigned int uVert = 0;

	//Vertex vertex;	 //Temp vertex
	unify::TexCoords uvLength;
	unify::TexCoords uv;
	
	uvLength = texArea.dr - texArea.ul;

	for( c = 0; c < (rc.column + 1); c++ )
	{
		float v = (1.0f / (rc.column)) * c;		// is unit
		uv.v = (texArea.ul.v + ((1.0f - v) * uvLength.v));

		for( r = 0; r < (rc.row + 1); r++ )
		{
			float h = (1.0f / (rc.row)) * r;	// is unit
			uv.u = texArea.ul.u + (uvLength.u * h);
			
			unify::V3< float > vPos( fStartX + (h * size.width), constant, fStartY + (v * size.height) );
			unify::V3< float > vNormal( 0, 1, 0 );
			unify::Color diffuse( unify::Color::ColorWhite() );
			vd->WriteVertex( lock, uVert, positionE, vPos );
			vd->WriteVertex( lock, uVert, normalE, vNormal );
			vd->WriteVertex( lock, uVert, texE, uv );
			vd->WriteVertex( lock, uVert, diffuseE, diffuse );
			uVert++;
		}
	}

	set.GetVertexBuffer().Create( vertexCount, vd, vertices.get() );

	// Fill in indices...
	std::vector< Index32 > indices( indexCount );

	unsigned int uInd = 0;
	unsigned int uSegmentsH = rc.row + 1;	// Number of segments
	for( c = 0; c < rc.column; c++ )
	{
		for( r = 0; r < (int)uSegmentsH; r++ )
		{
			indices[uInd++] = (Index32)((uSegmentsH * c) + r);
			indices[uInd++] = (Index32)((uSegmentsH * (c + 1)) + r);
		}
		if( c < (rc.column - 1) )
		{
			indices[uInd++] = (Index32)((uSegmentsH * (c + 2)) - 1);
			indices[uInd++] = (Index32)(uSegmentsH * (c + 1));
		}
	}

	IndexBuffer & ib = set.GetIndexBuffer();
	ib.Create( indexCount, (Index32*)&indices[0], BufferUsage::Default );

	m_rc = rc;

	ComputeBounds();

	if ( parameters.Exists( "heightmap" ) )
	{
		TextureOpMap tom = parameters.Get< TextureOpMap >( "heightmap" );
		ApplyHeightMap( tom );
	}

	if ( parameters.Exists( "alphamap" ) )
	{
		TextureOpMap tom = parameters.Get< TextureOpMap >( "alphamap" );
		ApplyAlphaMap( tom );
	}
}

bool Terra::ApplyHeightMap( TextureOpMap tom )
{
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
		vd->ReadVertex( lock, vert, positionE, vPos );

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
		vd->WriteVertex( lock, vert, positionE, vPos );
	}
	tom.texture->UnlockRect( 0 );

	set.GetVertexBuffer().Unlock();

	GenerateNormals();
	ComputeBounds();

	return true;
}


bool Terra::ApplyAlphaMap( TextureOpMap tom )
{
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
		vd->ReadVertex( lock, vert, positionE, vPos );

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
		vd->ReadVertex( lock, vert, diffuseE, colDiffuse );
		unify::ColorUnit colUnit;
		colUnit = colDiffuse;

		colUnit.a = result.SumComponents();
		colDiffuse = colUnit;

		// Update vertex in buffer...
		vd->WriteVertex( lock, vert, diffuseE, colDiffuse );

	}
	tom.texture->UnlockRect( 0 );

	set.GetVertexBuffer().Unlock();

	GenerateNormals();
	ComputeBounds();

	return true;
}


bool Terra::ApplyTextureMap( unsigned int dwMember, const unify::TexArea * pTexArea )
{
	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexElement positionE = CommonVertexElement::Position();

	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

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
			//vertvd->WriteVertex( lock, uVert, &unify::TexCoords( pTexArea->ul.u + (cMul.u * r), pTexArea->dr.v - (cMul.v * c) ), (FVF::TYPE)dwMember );
			uVert++;
		}
	}
	set.GetVertexBuffer().Unlock();

	return true;		
}

void Terra::GenerateNormals( bool bUseSelf )
{
	unify::DataLock lock;
	BufferSet & set = m_primitiveList.GetBufferSet( 0 ); // TODO: hard coded (perhaps I could even move this to a function of PL, like take a sudo-shader function?).
	set.GetVertexBuffer().Lock( lock );

	VertexDeclaration::ptr vd = set.GetVertexBuffer().GetVertexDeclaration();
	VertexElement positionE = CommonVertexElement::Position();
	VertexElement normalE = CommonVertexElement::Normal();

	unsigned int h, v;

	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

	for( v = 0; v < rc.column; v++ )
	{
		for( h = 0; h < rc.row; h++ )
		{
			unify::V3< float > vAxis[3];	// X, Y, Z
			
			unify::V3< float > vPosCenter;
			unify::V3< float > vPosX, vPosZ;

			vd->ReadVertex( lock, (v * rc.row) + h, positionE, vPosCenter );
	
			// X
			if( h == rc.row - 1 )	// At the far right edge...
			{
				vd->ReadVertex( lock, (v * rc.row) + (h - 1), positionE, vPosX );
				vPosX.y = vPosCenter.y;
				vAxis[0] = vPosCenter - vPosX;
			}
			else
			{
				vd->ReadVertex( lock, (v * rc.row) + (h + 1), positionE, vPosX );
				if( h == 0 ) vPosX.y = vPosCenter.y;	// left column
				vAxis[0] = vPosX - vPosCenter;
			}

			// Z
			if( v == rc.column - 1 )	// top row
			{
				vd->ReadVertex( lock, ((v - 1) * rc.row) + h, positionE, vPosZ );
				vPosZ.y = vPosCenter.y;
				vAxis[2] = vPosZ - vPosCenter;
			}
			else
			{
				vd->ReadVertex( lock, ((v + 1) * rc.row) + h, positionE, vPosZ );				
				if( v == 0 ) vPosZ.y = vPosCenter.y;	// bottom row		
				vAxis[2] = vPosCenter - vPosZ;
			}

			// Y
			vAxis[1].V3Cross( vAxis[0], vAxis[2] );

			vAxis[0].Normalize();
			vAxis[1].Normalize();
			vAxis[2].Normalize();

			unify::Matrix mRot( unify::Matrix::MatrixIdentity() );

			mRot.SetLeft( vAxis[0] );
			mRot.SetUp( vAxis[1] );
			mRot.SetForward( vAxis[2] );

			// Compute normal...
			unify::V3< float > vNormal = unify::V3< float >(0,1,0);

			mRot.TransformCoord( vNormal );
			vNormal.Normalize();
			
			vd->WriteVertex( lock, (v * rc.row) + h, normalE, vNormal );
		}
	}
	set.GetVertexBuffer().Unlock();
}

bool Terra::Smooth( unsigned int uFlags )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

	float * pfDepthArray = new float[ rc.column * rc.row ];
	if( ! pfDepthArray )
	{
		return FALSE;
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
				vd->ReadVertex( lock, (r-1) + (c * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Right vertex...
			if( r < (rc.row-1) )
			{
				vd->ReadVertex( lock, (r+1) + (c * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Up vertex...
			if( c > 0 )
			{
				vd->ReadVertex( lock, (r) + ((c-1) * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}
			// Down vertex...
			if( c < (rc.column-1) )
			{
				vd->ReadVertex( lock, (r) + ((c+1) * rc.row), positionE, vVecIn );
				fDepth += vVecIn.y;
				fParts += 1;
			}

			// Assign present vertex...
			vd->ReadVertex( lock, (r + c * rc.row), positionE, vVecIn );

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
			vd->ReadVertex( lock, r + c * rc.row, positionE, vec );
			vec.y = pfDepthArray[r + (c * rc.row)];
			vd->WriteVertex( lock, r + c * rc.row, positionE, vec );
		}
	}

	delete [] pfDepthArray;
	pfDepthArray = 0;

	set.GetVertexBuffer().Unlock();
	return true;
}

bool Terra::ApplyTransparent( unsigned int uFlags, float fValue, float fTolerance )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

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
			vd->ReadVertex( lock, r + c * rc.row, positionE, vVecIn );
			fDepth = vVecIn.y;

			if( unify::CheckFlag( uFlags, TRANS_BELOW ) && fDepth < fValue )
			{
				unify::Color colorIn;
				vd->ReadVertex( lock, r + c * rc.row, diffuseE, colorIn );
				unify::ColorUnit colorMix = colorIn;
				colorMix.a = 0.0f;
				colorIn = colorMix;
				vd->WriteVertex( lock, r + c * rc.row, diffuseE, colorIn );
			}
			else if( unify::CheckFlag( uFlags, TRANS_ABOVE ) && fDepth > fValue )
			{
				unify::Color colorIn;
				vd->ReadVertex( lock, r + c * rc.row, diffuseE, colorIn );
				unify::ColorUnit colorMix = colorIn;
				colorMix.a = 0.0f;
				colorIn = colorMix;
				vd->WriteVertex( lock, r + c * rc.row, diffuseE, colorIn );
			}
		}
	}

	set.GetVertexBuffer().Unlock();

	return true;
}

// Fix either ROWS or COLUMNS to the average of both sides.
bool Terra::MakeWrappable( unsigned int uFlags )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

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
			vd->ReadVertex( lock, (r) + (0 * rc.row), positionE, vec1 );
			vd->ReadVertex( lock, (r) + ((rc.column - 1) * rc.row), positionE, vec2 );

			fDepth = (vec1.y + vec2.y) / 2.0f;
			vec1.y = fDepth;
			vec2.y = fDepth;

			vd->WriteVertex( lock, (r) + (0 * rc.row), positionE, vec1 );
			vd->WriteVertex( lock, (r) + ((rc.column - 1) * rc.row), positionE, vec2 );
		}
	}

	// Columns...
	if( unify::CheckFlag(uFlags,WRAP_COLUMNS) )
	{
		for( c = 0; c < rc.column; c++ )
		{
			unify::V3< float > vec1, vec2;
			vd->ReadVertex( lock, (0) + (c * rc.row), positionE, vec1 );
			vd->ReadVertex( lock, (rc.row - 1) + (c * rc.row), positionE, vec2 );
			
			fDepth = (vec1.y + vec2.y) / 2.0f;
			vec1.y = fDepth;
			vec2.y = fDepth;

			vd->WriteVertex( lock, (0) + (c * rc.row), positionE, vec1);
			vd->WriteVertex( lock, (rc.row - 1) + (c * rc.row), positionE, vec2 );
		}
	}

	// Corners...
	if( unify::CheckFlag(uFlags,WRAP_ROWS) && unify::CheckFlag(uFlags,WRAP_COLUMNS) )
	{
		unify::V3< float > vec[4];
		vd->ReadVertex( lock, (0) + (0 * rc.row), positionE, vec[0] );
		vd->ReadVertex( lock, (rc.row - 1) + (0 * rc.row), positionE, vec[1] );
		vd->ReadVertex( lock, (0) + ((rc.column - 1) * rc.row), positionE, vec[2] );
		vd->ReadVertex( lock, (rc.row - 1) + ((rc.column - 1) * rc.row), positionE, vec[3] );

		fDepth = (vec[0].y + vec[1].y + vec[2].y + vec[3].y) / 4.0f;

		vec[0].y = fDepth;
		vec[1].y = fDepth;
		vec[2].y = fDepth;
		vec[3].y = fDepth;

		vd->WriteVertex( lock, (rc.row - 1) + (0 * rc.row), positionE, vec[1] );
		vd->WriteVertex( lock, (0) + ((rc.column - 1) * rc.row), positionE, vec[2] );
		vd->WriteVertex( lock, (rc.row - 1) + ((rc.column - 1) * rc.row), positionE, vec[3] );
	}

	set.GetVertexBuffer().Unlock();
	
	return true;
}

// Set a column or row to a given depth value.
bool Terra::FixSide( unsigned int uFlags, float fToDepth )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );
	
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
			vd->ReadVertex( lock, (r) + (0 * rc.row), positionE, vec );
			vec.y = fToDepth;
			vd->WriteVertex( lock, (r) + (0 * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			vd->ReadVertex( lock, (r) + ((rc.column - 1) * rc.row), positionE, vec );
			vec.y = fToDepth;
			vd->WriteVertex( lock, (r) + ((rc.column - 1) * rc.row), positionE, vec );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			vd->ReadVertex( lock, (0) + (c * rc.row), positionE, vec );
			vec.y = fToDepth;
			vd->WriteVertex( lock, (0) + (c * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			vd->ReadVertex( lock, (rc.row - 1) + (c * rc.row), positionE, vec );
			vec.y = fToDepth;
			vd->WriteVertex( lock, (rc.row - 1) + (c * rc.row), positionE, vec );
		}
	}

	set.GetVertexBuffer( ).Unlock();
	
	return true;
}

// Align a side to a given terra's side
bool Terra::AlignSide( unsigned int uFlags, Terra * pTerraIn )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

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
			vd->ReadVertex( lock, (r) + (0 * rc.row), positionE, vec );
			vdIn->ReadVertex( lockIn, (r) + ((rc.column-1) * rc.row), positionE, vecin );
			vec.y = vecin.y;
			vd->WriteVertex( lock, (r) + (0 * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			vd->ReadVertex( lock, (r) + ((rc.column-1) * rc.row), positionE, vec );
			vdIn->ReadVertex( lockIn, (r) + (0 * rc.row), positionE, vecin );
			vec.y = vecin.y;
			vd->WriteVertex( lock, (r) + ((rc.column-1) * rc.row), positionE, vec );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			vd->ReadVertex( lock, (0) + (c * rc.row), positionE, vec );
			vdIn->ReadVertex( lockIn, (rc.row-1) + (c * rc.row), positionE, vecin );
			vec.y = vecin.y;
			vd->WriteVertex( lock, (0) + (c * rc.row), positionE, vec );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			vd->ReadVertex( lock, (rc.row-1) + (c * rc.row), positionE, vec );
			vdIn->ReadVertex( lockIn, (0) + (c * rc.row), positionE, vecin );
			vec.y = vecin.y;
			vd->WriteVertex( lock, (rc.row-1) + (c * rc.row), positionE, vec );
		}
	}

	pTerraIn->m_primitiveList.GetBufferSet( 0 ).GetVertexBuffer().Unlock();
	set.GetVertexBuffer().Unlock();
	
	return true;
}

// Set the normals on the side(s) specified to a given normal
bool Terra::NormalSide( unsigned int uFlags, const unify::V3< float > & normal )
{
	// Vertex rows and columns...
	unify::RowColumn< unsigned int > rc( m_rc.row + 1, m_rc.column + 1 );

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
			vd->WriteVertex( lock, (r) + (0 * rc.row), normalE, normal );
		}

		if( unify::CheckFlag(uFlags, SIDE_UP) )
		{
			vd->WriteVertex( lock, (r) + ((rc.column-1) * rc.row), normalE, normal );
		}
	}

	// Columns...
	for( c = 0; c < rc.column; c++ )
	{
		if( unify::CheckFlag(uFlags, SIDE_LEFT) )
		{
			vd->WriteVertex( lock, (0) + (c * rc.row), normalE, normal );
		}

		if( unify::CheckFlag(uFlags, SIDE_RIGHT) )
		{
			vd->WriteVertex( lock, (rc.row-1) + (c * rc.row), normalE, normal );
		}
	}

	set.GetVertexBuffer().Unlock();
	
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

void Terra::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
}

void Terra::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	m_primitiveList.Render( renderInfo );

	// Render Normals...
	//RenderNormals();
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
		vd->ReadVertex( lock, dwVertex, &vFromPos, FVF::XYZ );
		vd->ReadVertex( lock, dwVertex, &vFromNorm, FVF::Normal );

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

const unify::RowColumn< unsigned int > & Terra::GetRC() const
{
	return m_rc;
}

PrimitiveList & Terra::GetPrimitiveList()
{
	return m_primitiveList;
}
