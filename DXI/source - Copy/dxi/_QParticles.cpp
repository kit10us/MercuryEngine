/* Copyright (c) 2002 - 2010 Quentin S. Smith
 * All Rights Reserved
 */

#include "dxi/QParticles.h"
#include "dxi/QVertexBuffer.h"
#include "dxi/QSystem.h"
#include <qlib/Common.h>

using namespace dxi;

// TODO: Revamp this system!

TVertexFormat PSIZEVERTEX::FORMAT = FVF::XYZ | FVF::PSize | FVF::Diffuse;
TVertexFormat POINTVERTEX::FORMAT = FVF::XYZ | FVF::Diffuse;
TVertexFormat COLORVERTEX::FORMAT = FVF::XYZ | FVF::Diffuse | FVF::Tex1;

/////
// QEmitter
/////


/////
// QTemplate
/////
QTemplate::QTemplate()
{
	// Particle List...
	m_pFirstParticle	= 0;
	m_pLastParticle		= 0;
	m_dwNumParticles	= 0;
	m_dwMaxParticles	= 0;

	// Particle info
	m_dwParticleType	= PARTICLE_TICK3D;
	m_dwLineType		= LINE_FROMLASTPOSITION;

	m_vForce.Zero();

	m_fTickSize		= 4.0f;
	
	// Point Sprite variables...
	m_dwFlush			= 512;
	m_dwDiscard			= 2048;
	m_fPointSize_PS		= 1.0f;
	m_fPointSizeMin_PS	= 0.0f;
	m_fA_PS				= 0.0f;	//1?
	m_fB_PS				= 0.0f;
	m_fC_PS				= 1.0f;	//0?

	// Distance OP
	//m_dwOnDistance	= ONDISTANCE_NOTHING;
	//m_fDistance		= 1;

	// Decay size
	m_iDecaySOP		= DECAYSOP_NONE;
	m_fSizeDecay	= 0;

	// Decay diffuse
	m_iDecayDOP		= DECAYDOP_NONE;
	m_cuDecay		= unify::ColorUnit::Zero();
	
	m_dwMinLifeTime = 200;
	m_dwMaxLifeTime = 200;
	
	m_fLength		= 1.0f;
	//m_pTexture		= 0;
	//m_iSpriteH		= -1;
	//m_iSpriteV		= -1;

	m_fMinSize		= 1;
	m_fMaxSize		= 1;

	m_iBirthDOP		= BIRTHDOP_NONE;
	m_cuBirth1		= unify::ColorUnit::White();
	m_cuBirth2		= unify::ColorUnit::White();
	
	m_iBirth_DifLow = 0;
	
	// POP
	m_pFirstPOP = 0;
	m_pLastPOP = 0;
	m_dwPOPCount = 0;

	m_dwEmitterReferenceCount = 0;
	m_dwParticleReferenceCount = 0;
	m_pNext = 0;
}

QTemplate::~QTemplate()
{
	ReleaseData();
	KillParticles();
	KillPOPs();
}

unify::Bool QTemplate::Process()
{
	QParticle* pParticle;

	if( !m_dwMaxParticles ) return TRUE;

	// Don't process if we've just resumed
	if( System()->GetState( SY_RESUME ) ) return TRUE;

	static unify::Float fElapsed	= 0;
	if( fElapsed )
	{
		// Average elpased to smooth
		fElapsed += System()->Elapsed(); 
		fElapsed *= 0.5f;
	}
	else fElapsed = System()->Elapsed();

	// Process the particles
	pParticle = m_pFirstParticle;
	QParticle* pParent	= 0;			// NULL == m_pFirstParticle
	QParticle* pNext	= 0;	
	while( pParticle )
	{
		// Get the next particle...
		pNext = pParticle->pNext;

		pParticle = AgeParticle( pParticle, fElapsed );

		if( pParticle ) pParent = pParticle;

		// Move on to the next particle...
		pParticle = pNext;	// Our next is now the current particle
	}

	return TRUE;
}

// Render particles...
void QTemplate::Render( RenderInfo & renderInfo )
{
	/*
		Note: First of all, AlphaBlending is handled by the base object, however,
		aditional or changes in blending can be introduced.
	*/

	m_dwRenderedParticles = 0;

	if( ! m_pFirstParticle )
	{
		return;	// No particles to render
	}
	
	// Our result
	HRESULT hr = S_OK;

	// Get the status of the current render states...
	unify::DWord StateBackup[5];
	GetDevice()->GetRenderState( D3DRS_LIGHTING,			&StateBackup[3] );
	GetDevice()->GetRenderState( D3DRS_ZWRITEENABLE,		&StateBackup[4] );

	//////////////
	/*
	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, false );
	GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	GetDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	GetDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	*/
	//////////////


	QParticle * pParticle = m_pFirstParticle;

	switch( m_dwParticleType )
	{
	default:
		break;
	case PARTICLE_TICK3D:
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			POINTVERTEX* pVertex = 0;
			unify::Float fHalfSize;

			fHalfSize = m_fTickSize * 0.5f;


			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}

			while( pParticle )
			{
				// Setup the current Vertex to the current Particle
				// LINE ONE
				pVertex->p = pParticle->vPosition - unify::V3< unify::Float >( fHalfSize, 0, 0 );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;

				pVertex->p = pParticle->vPosition + unify::V3< unify::Float >( fHalfSize, 0, 0 );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;

				// LINE TWO
				pVertex->p = pParticle->vPosition - unify::V3< unify::Float >( 0, fHalfSize, 0 );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;

				pVertex->p = pParticle->vPosition + unify::V3< unify::Float >( 0, fHalfSize, 0 );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;

				// LINE TWO
				pVertex->p = pParticle->vPosition - unify::V3< unify::Float >( 0, 0, fHalfSize );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;

				pVertex->p = pParticle->vPosition + unify::V3< unify::Float >( 0, 0, fHalfSize );
				pVertex->diffuse = pParticle->GetColor();
				pVertex++;


				// Next particle...
				pParticle = pParticle->pNext;
				m_dwRenderedParticles++;
			}

			// Unlock Vertex Buffer
			m_VB.Unlock();

			// Render points...
			m_VB.Render( QRenderMethod( PT_LineList, 0, 3 * m_dwRenderedParticles ) );
		}
		break;

	case PARTICLE_POINT:
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			POINTVERTEX* pVertex = 0;

			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex ) 
			{
				throw qlib::Exception( "Failed to render particles!" );
			}

			while( pParticle )
			{
				// Setup the current Vertex to the current Particle
				pVertex->p			= pParticle->vPosition;
				pVertex->diffuse	= pParticle->GetColor();

				// Next particle...
				pParticle = pParticle->pNext;
				pVertex++;
				m_dwRenderedParticles++;
			}

			// Unlock Vertex Buffer
			m_VB.Unlock();

			// Render points...
			m_VB.Render( QRenderMethod( PT_PointList, 0, m_dwRenderedParticles ) );
		}
		break;

	case PARTICLE_LINE:
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			POINTVERTEX* pVertex = 0;

			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}

			while( pParticle )
			{
				// Setup the current Vertex to the current Particle
				if( pParticle->vLastPosition == pParticle->vPosition )
				{
					pVertex->p = pParticle->vPosition;
					pVertex->diffuse = pParticle->GetColor();
					pVertex++;

					pVertex->p = pParticle->vPosition;
					pVertex->diffuse = pParticle->GetColor();
					pVertex++;
				}
				else
				{
					unify::V3< unify::Float > vec;
					switch( m_dwLineType )
					{
					default:
					case LINE_FROMLASTPOSITION:
						pVertex->p = pParticle->vPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;

						pVertex->p = pParticle->vLastPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;
						break;
						
					case LINE_LENGTH:
						vec = pParticle->vLastPosition - pParticle->vPosition;
						vec.Normalize();
						vec *= pParticle->pMyTemplate->GetParticleLength();

						pVertex->p = pParticle->vPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;

						pVertex->p = pParticle->vPosition + vec;
						//pVertex->p = pParticle->vPosition + pParticle->vVelocity;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;
						break;

					case LINE_VELOCITY:
						pVertex->p = pParticle->vPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;

						pVertex->p = pParticle->vPosition + pParticle->vVelocity;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;
						break;

					case LINE_ACCELERATION:
						pVertex->p = pParticle->vPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;

						pVertex->p = pParticle->vPosition + pParticle->vAcceleration;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;
						break;

					case LINE_LENGTHVELOCITY:
						vec = pParticle->vLastPosition - pParticle->vPosition;
						vec.Normalize();
						vec *= pParticle->pMyTemplate->GetParticleLength();
						vec *= pParticle->vVelocity.Length();


						pVertex->p = pParticle->vPosition;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;

						pVertex->p = pParticle->vPosition + vec ;
						//pVertex->p = pParticle->vPosition + pParticle->vVelocity;
						pVertex->diffuse = pParticle->GetColor();
						pVertex++;
						break;
					}
				}

				// Next particle...
				pParticle = pParticle->pNext;
				m_dwRenderedParticles++;
			}

			// Unlock Vertex Buffer
			m_VB.Unlock();

			// Render points...
			m_VB.Render( QRenderMethod( PT_LineList, 0, m_dwRenderedParticles ) );
			break;
		}

	case PARTICLE_BILLBOARD:	// INDEXED
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			unify::Matrix matIdentity;
			matIdentity.Identity();
			unify::Matrix matView, matWorld;
			
			// Get backups of the world and view matrices
			GetDevice()->GetTransform( D3DTS_WORLD, matWorld.GetD3DXMatrix() );
			GetDevice()->GetTransform( D3DTS_VIEW, matView.GetD3DXMatrix() );

			// Dissable view matrix
			GetDevice()->SetTransform( D3DTS_VIEW, matIdentity.GetD3DXMatrix() );

			// Disable world transformation:
			GetDevice()->SetTransform( D3DTS_WORLD, matIdentity.GetD3DXMatrix() );

			
			// Set the vertex shader
			//GetDevice()->SetFVF( D3DFVF_COLORVERTEX );

			unify::Float fVP;

			// Set the texture 
			if( m_pImage->GetTexture() )
			{
				m_pImage->GetTexture()->UseTexture( 0 );
			}

			TIndex16 * pIndex;
			COLORVERTEX* pVertex = 0;
			m_VB.LockIndices( (void**)&pIndex );
			m_VB.LockVertices( (void**)&pVertex );

			if( ! pIndex || ! pVertex )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}
			

			unify::Color diffuse;
			unify::V3< unify::Float > vTransPos;
			unify::Word	wIndex		= 0;
			unify::Word	wParticle	= 0;

			unify::V3< unify::Float >	vPos;

			while( pParticle )
			{					
				// Set the color...
				diffuse	= pParticle->GetColor();

				// Apply the view matrix to the position vector:
				vPos = pParticle->vPosition;
				// There seems to be a problem with the direction of the billboards
				// So we do this (seems to fix it)
				vPos.x *= -1;
				vPos.z *= -1;


				vTransPos.TransformCoord( vPos, matView ); // SPEED HIT

				fVP				= 0.5f * pParticle->fSize;

				// upper left
				pVertex->p = unify::V3< unify::Float >( vTransPos.x - fVP, vTransPos.y + fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords		= m_pImage->TexArea()->ul;
				pVertex++;

				// upper right
				pVertex->p = unify::V3< unify::Float >( vTransPos.x + fVP, vTransPos.y + fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->dr.u;
				pVertex->coords.v	= m_pImage->TexArea()->ul.v;
				pVertex++;
				
				// lower left
				pVertex->p = unify::V3< unify::Float >( vTransPos.x - fVP, vTransPos.y - fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->ul.u;
				pVertex->coords.v	= m_pImage->TexArea()->dr.v;
				pVertex++;

				// lower right
				pVertex->p = unify::V3< unify::Float >( vTransPos.x + fVP, vTransPos.y - fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords	= m_pImage->TexArea()->dr;
				pVertex++;

				// Index buffer...
				unify::Word p4 = wParticle * 4;
				pIndex[wIndex++] = (p4) + 0;
				pIndex[wIndex++] = (p4) + 1;
				pIndex[wIndex++] = (p4) + 2;
				
				pIndex[wIndex++] = (p4) + 1;
				pIndex[wIndex++] = (p4) + 3;
				pIndex[wIndex++] = (p4) + 2;
			
				pParticle = pParticle->pNext;
				
				m_dwRenderedParticles++;
				wParticle++;
			}
			m_VB.Unlock();

			m_VB.Render( QRenderMethod( PT_TriangleList, 0, 0, m_dwRenderedParticles * 4, 0, wIndex, m_dwRenderedParticles * 2 ) );

			// Set the texture to NULL
			GetDevice()->SetTexture( 0, 0 );

			// Restore the matrices
			GetDevice()->SetTransform( D3DTS_WORLD, matWorld.GetD3DXMatrix() );
			GetDevice()->SetTransform( D3DTS_VIEW, matView.GetD3DXMatrix() );
		 } 
		 break;

	case PARTICLE_BILLBOARD2:	// Triangle Lists
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			unify::Matrix matIdentity, matView, matWorld;
			matIdentity.Identity();
			
			// Get backups of the world and view matrices
			GetDevice()->GetTransform( D3DTS_WORLD, matWorld.GetD3DXMatrix() );
			GetDevice()->GetTransform( D3DTS_VIEW, matView.GetD3DXMatrix() );

			// Dissable view matrix
			GetDevice()->SetTransform( D3DTS_VIEW, matIdentity.GetD3DXMatrix() );

			// Disable world transformation:
			GetDevice()->SetTransform( D3DTS_WORLD, matIdentity.GetD3DXMatrix() );

			// Set the vertex shader
			//GetDevice()->SetFVF( D3DFVF_COLORVERTEX );

			unify::Float fVP;

			// Set the texture 
			if( m_pImage->GetTexture() )
			{
				m_pImage->GetTexture()->UseTexture( 0 );
			}

			COLORVERTEX* pVertex = 0;
			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}
			
			unify::Color diffuse;
			unify::V3< unify::Float > vTransPos;
			unify::Word	wParticle	= 0;

			unify::V3< unify::Float >	vPos;

			while( pParticle )
			{
					
				// Set the color...
				diffuse	= pParticle->GetColor();

				// Apply the view matrix to the position vector:
				vPos = pParticle->vPosition;
				// There seems to be a problem with the direction of the billboards
				// So we do this (seems to fix it)
				vPos.x *= -1;
				vPos.z *= -1;

				vTransPos.TransformCoord( vPos, matView ); // SPEED HIT

				fVP	= 0.5f * pParticle->fSize;

				// TRIANGLE 1
				// upper left
				pVertex->p = unify::V3< unify::Float >( vTransPos.x - fVP, vTransPos.y + fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords	= m_pImage->TexArea()->ul;
				pVertex++;
				// upper right
				pVertex->p = unify::V3< unify::Float >( vTransPos.x + fVP, vTransPos.y + fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->dr.u;
				pVertex->coords.v	= m_pImage->TexArea()->ul.v;
				pVertex++;
				// lower left
				pVertex->p = unify::V3< unify::Float >( vTransPos.x - fVP, vTransPos.y - fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->ul.u;
				pVertex->coords.v	= m_pImage->TexArea()->dr.v;
				pVertex++;
				
				// TRIANGLE 2
				// upper right
				pVertex->p = unify::V3< unify::Float >( vTransPos.x + fVP, vTransPos.y + fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->dr.u;
				pVertex->coords.v	= m_pImage->TexArea()->ul.v;
				pVertex++;
				// lower right
				pVertex->p = unify::V3< unify::Float >( vTransPos.x + fVP, vTransPos.y - fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords	= m_pImage->TexArea()->dr;
				pVertex++;
				// lower left
				pVertex->p = unify::V3< unify::Float >( vTransPos.x - fVP, vTransPos.y - fVP, vTransPos.z );
				pVertex->diffuse	= diffuse;
				pVertex->coords.u	= m_pImage->TexArea()->ul.u;
				pVertex->coords.v	= m_pImage->TexArea()->dr.v;
				pVertex++;
			
				pParticle = pParticle->pNext;
				
				m_dwRenderedParticles++;
				wParticle++;
			}
			m_VB.Unlock();

			m_VB.Render( QRenderMethod( PT_TriangleList, 0, m_dwRenderedParticles * 2 ) );

			// Set the texture to NULL
			GetDevice()->SetTexture( 0, 0 );

			// Restore the matrices
			GetDevice()->SetTransform( D3DTS_WORLD, matWorld.GetD3DXMatrix() );
			GetDevice()->SetTransform( D3DTS_VIEW, matView.GetD3DXMatrix() );
		 } 
		 break;
	
	case PARTICLE_GEOMETRY:
		{
			if( ! m_pParticleGeometry )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}
			
			unify::Matrix mWorldOriginal;
			GetDevice()->GetTransform( D3DTS_WORLD, mWorldOriginal.GetD3DXMatrix() );

			unify::Matrix mWorldNew;

			// Loop through all particles...
			while( pParticle )
			{

				unify::Matrix mTranslate;
				mWorldNew.Translate( pParticle->vPosition );

				mWorldNew = mWorldNew * mWorldOriginal;
				GetDevice()->SetTransform( D3DTS_WORLD, mWorldNew.GetD3DXMatrix() );
				
				m_pParticleGeometry->Render();

				pParticle = pParticle->pNext;
				m_dwRenderedParticles++;
			}

			GetDevice()->SetTransform( D3DTS_WORLD, mWorldOriginal.GetD3DXMatrix() );
		}
		break;

	/*
	case PARTICLE_PSPRITE: {

		// Set the texture 
		if( IDFAILED(m_pImage->idTexture) )
		{
			Texture( m_pImage->idTexture )->SetTexture( 0 );
		}

		// Setup render states...
		GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, true );
		GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  true );
		GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(m_fPointSize_PS) );
		GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(m_fPointSizeMin_PS) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(m_fA_PS) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(m_fB_PS) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(m_fC_PS) );

		// Set up the vertex buffer to be rendered
		GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(POINTVERTEX) );
		GetDevice()->SetFVF( D3DFVF_POINTVERTEX );

		POINTVERTEX* pVertices;
		unify::DWord        dwNumParticlesToRender = 0;


		// Lock the vertex buffer.  We fill the vertex buffer in small
		// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
		// each chunk, we call DrawPrim, and lock the next chunk.  When
		// we run out of space in the vertex buffer, we start over at
		// the beginning, using D3DLOCK_DISCARD.

		m_dwBase += m_dwFlush;

		if(m_dwBase >= m_dwDiscard)
			m_dwBase = 0;

		hr = m_pVB->Lock( m_dwBase * sizeof(POINTVERTEX), m_dwFlush * sizeof(POINTVERTEX),
			(void**) &pVertices, m_dwBase ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD );
		if( FAILED(hr) ) return FALSE;
		

		// Render each particle
		while( pParticle )
		{
			pVertices->p		= pParticle->vPosition;
			pVertices->diffuse	= pParticle->GetColor();
			pVertices++;

			// When the number of particles is 'Flush', render particles...
			if( ++dwNumParticlesToRender == m_dwFlush )
			{
				// Done filling this chunk of the vertex buffer.  Lets unlock and
				// draw this portion so we can begin filling the next chunk.

				m_pVB->Unlock();

				if( FAILED(hr = GetDevice()->DrawPrimitive( D3DPT_PointList, m_dwBase, dwNumParticlesToRender)) )
				{
					return FALSE;
				}

				// Lock the next chunk of the vertex buffer.  If we are at the 
				// end of the vertex buffer, DISCARD the vertex buffer and start
				// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
				// continue filling the VB while the previous chunk is drawing.
				m_dwBase += m_dwFlush;

				if( m_dwBase >= m_dwDiscard )
					m_dwBase = 0;

				if( FAILED(hr = m_pVB->Lock( m_dwBase * sizeof(POINTVERTEX), m_dwFlush * sizeof(POINTVERTEX),
					(void**) &pVertices, m_dwBase ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)) )
				{
					return FALSE;
				}

			}

			pParticle = pParticle->pNext;
			m_dwRenderedParticles++;
		}

		// Unlock the vertex buffer
		m_pVB->Unlock();

		// Render any remaining particles
		if( dwNumParticlesToRender )
		{
			hr = GetDevice()->DrawPrimitive( D3DPT_PointList, m_dwBase, dwNumParticlesToRender );
				if( FAILED(hr) ) return FALSE;
		}

		// Reset render states
		GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, false );
		GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  false );

		GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(1.0f) );
		GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(0.0f) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(1.0f) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(0.0f) );
		GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(0.0f) );

		GetDevice()->SetTexture( 0, NULL );
							} break;
	*/

	case PARTICLE_POINTSPRITE:
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			// Set the texture 
			if( m_pImage->GetTexture() )
			{
				m_pImage->GetTexture()->UseTexture( 0 );
			}

			// Setup render states...
			GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, true );
			GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  true );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(m_fPointSize_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(m_fPointSizeMin_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(m_fA_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(m_fB_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(m_fC_PS) );

			POINTVERTEX* pVertex = 0;
			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex ) 
			{
				throw qlib::Exception( "Failed to render particles!" );
			}

			while( pParticle )
			{
				// Setup the current Vertex to the current Particle
				pVertex->p			= pParticle->vPosition;
				pVertex->diffuse	= pParticle->GetColor();

				// Next particle...
				pParticle = pParticle->pNext;
				pVertex++;
				m_dwRenderedParticles++;
			}

			// Unlock Vertex Buffer
			m_VB.Unlock();

			// Render points...
			m_VB.Render( QRenderMethod( PT_PointList, 0, m_dwRenderedParticles ) );

			// Reset render states
			GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, false );
			GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  false );

			GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(1.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(0.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(1.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(0.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(0.0f) );

			GetDevice()->SetTexture( 0, 0 );
		}
		break;

	case PARTICLE_PSIZE:
		{
			GetDevice()->SetRenderState( D3DRS_LIGHTING,			FALSE );
			GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		FALSE );

			// Set the texture 
			if( m_pImage->GetTexture() )
			{
				m_pImage->GetTexture()->UseTexture( 0 );
			}

			// Setup render states...
			GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, true );
			GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  true );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(m_fPointSize_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(m_fPointSizeMin_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(m_fA_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(m_fB_PS) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(m_fC_PS) );

			PSIZEVERTEX * pVertex = 0;
			m_VB.LockVertices( (void**)&pVertex );
			if( ! pVertex )
			{
				throw qlib::Exception( "Failed to render particles!" );
			}

			while( pParticle )
			{
				// Setup the current Vertex to the current Particle
				pVertex->p			= pParticle->vPosition;
				pVertex->diffuse	= pParticle->GetColor();
				pVertex->size		= pParticle->fSize;

				// Next particle...
				pParticle = pParticle->pNext;
				pVertex++;
				m_dwRenderedParticles++;
			}

			// Unlock Vertex Buffer
			m_VB.Unlock();

			// Render points...
			m_VB.Render( QRenderMethod( PT_PointList, 0, m_dwRenderedParticles ) );

			// Reset render states
			GetDevice()->SetRenderState( D3DRS_POINTSPRITEENABLE, false );
			GetDevice()->SetRenderState( D3DRS_POINTSCALEENABLE,  false );

			GetDevice()->SetRenderState( D3DRS_POINTSIZE,     qlib::FtoDW(1.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSIZE_MIN, qlib::FtoDW(0.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_A,  qlib::FtoDW(1.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_B,  qlib::FtoDW(0.0f) );
			GetDevice()->SetRenderState( D3DRS_POINTSCALE_C,  qlib::FtoDW(0.0f) );

			GetDevice()->SetTexture( 0, 0 );
		}
		break;
	/*

	case PARTICLE_TRIBOARD: {

		TMatrix matIdentity, matView, matWorld;
		qMatrixIdentity( &matIdentity );
		
		// Get backups of the world and view matrices
		GetDevice()->GetTransform( D3DTS_WORLD, &matWorld.mat );
		GetDevice()->GetTransform( D3DTS_VIEW, &matView.mat );

		// Dissable view matrix
		GetDevice()->SetTransform( D3DTS_VIEW, &matIdentity.mat );

		// Disable world transformation:
		GetDevice()->SetTransform( D3DTS_WORLD, &matIdentity.mat );

		
		// Set the vertex shader
		GetDevice()->SetFVF(QVERTEX);

		unify::Float fVP;

		// Set the texture 
		if( IDFAILED(m_pImage->idTexture) )
		{
			Texture( m_pImage->idTexture )->SetTexture( 0 );
		}

		// Lock the VB and assign particles...
		hr = m_pVB->Lock( 0, 0, (void**)&pData, 0 );
		QVertex* pVertex = (QVertex*)pData;

		unify::Color diffuse;
		unify::V3< unify::Float > vTransPos;
		unify::Float fSize;

		while( pParticle )
		{
				
			// Set the color...
			diffuse	= pParticle->GetColor();
			fSize = pParticle->fSize;

			// Apply the view matrix to the position vector:
			qV3TransformCoord( &vTransPos, &pParticle->vPosition, &matView );	// SPEED HIT

			fVP = 0.5f * fSize;

			// upper left
			pVertex->Pos( &V3< unify::Float >(vTransPos.x - fVP, vTransPos.y + fVP, vTransPos.z) );
			pVertex->Diffuse( diffuse );
			pVertex->Specular( unify::ColorWhite );
			pVertex->Coords( m_pImage->cUL.u, m_pImage->cUL.v );
			pVertex++;

			// upper right
			pVertex->Pos( &V3< unify::Float >(vTransPos.x + fVP + fSize, vTransPos.y + fVP, vTransPos.z) );
			pVertex->Diffuse( diffuse );
			pVertex->Specular( unify::ColorWhite );
			pVertex->Coords( m_pImage->cDR.u, m_pImage->cUL.v );
			pVertex++;
			
			// lower left
			pVertex->Pos( &V3< unify::Float >(vTransPos.x - fVP, vTransPos.y - (fVP + fSize), vTransPos.z) );
			pVertex->Diffuse( diffuse );
			pVertex->Specular( unify::ColorWhite );
			pVertex->Coords( m_pImage->cUL.u, m_pImage->cDR.v );
			pVertex++;

			pParticle = pParticle->pNext;
			m_dwRenderedParticles++;
		}
		m_pVB->Unlock();

		// Render Vertex Buffer...
		GetDevice()->SetStreamSource( 0, m_pVB, 0, sizeof(QVertex) );
		GetDevice()->DrawPrimitive( D3DPT_TriangleList, 0, m_dwRenderedParticles );

		// Set the texture to NULL
		GetDevice()->SetTexture( 0, NULL );

		// Restore the matrices
		GetDevice()->SetTransform( D3DTS_WORLD, &matWorld.mat );
		GetDevice()->SetTransform( D3DTS_VIEW, &matView.mat );

							 } break;
		*/
	}

	// Return the render states to they're previous states
	GetDevice()->SetRenderState( D3DRS_LIGHTING,			StateBackup[3] );
	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE,		StateBackup[4] );
}

void QTemplate::KillParticle( QParticle* pParticle, QParticle* pParent )
{
	// Make sure we're the owner of the particle...
	if( pParticle->pMyTemplate != this )
	{
		pParticle->pMyTemplate->KillParticle( pParticle, pParent );
		return;
	}

	// Remove us from the stream...
	// If no parent was given, check if we're the first, else find our parent!
	if( !pParent )
	{
		if( m_pFirstParticle == pParticle )	// Are we the first? (then our parent WOULD be NULL)
		{
			m_pFirstParticle = pParticle->pNext;
		}
		else	// Not the first, so some bastard didn't pass our parent! Now we need to call Jerry Springer...
		{
			pParent = m_pFirstParticle;
			while( pParent && pParent->pNext != pParticle ) pParent = pParent->pNext;
			// NOTE: We're not going to worry if we're not the host to the particle (too bad!)

			// Tell our parent our next is next...
			pParent->pNext = pParticle->pNext;
		}
	}
	else
	{
		// Tell our parent our next is next...
		pParent->pNext = pParticle->pNext;
	}

	// Check if we're last...
	if( m_pLastParticle == pParticle )
	{
		// Set out parent as last (even to null if we have no parent)
		m_pLastParticle = pParent;
	}



	/*
	// If we have a parent, set the parent's next from our's
	if( pParent )
	{
		pParent->pNext = pNext;
		if( !pNext )
		{
			m_pLastParticle = pParent;
		}
	}
	else	// Else, it's the first (set the first from the next)
	{
		m_pFirstParticle = pNext;
		if( !pNext ) m_pLastParticle = 0;
	}
	*/

	m_dwNumParticles--;
	delete pParticle;
}

// Allow a particle to be aged by fTime (in seconds)
// We now handle dying, but we don't destroy anything (to protect pointers)
QParticle* QTemplate::AgeParticle( QParticle *pParticle, unify::Float fTime )
{
	// Age particle
	pParticle->fAge += fTime;

	// Die on OLD
	//if( pParticle->fLifeTime && (pParticle->fAge >= pParticle->fLifeTime) )
	if( (pParticle->fAge >= pParticle->fLifeTime) )
	{
		KillParticle( pParticle );
		return NULL;
	}

	// Record the current position as last
	pParticle->vLastPosition = pParticle->vPosition;

	// Affect the velocity by FORCE (global to all particles)
	pParticle->vVelocity += m_vForce * fTime;

	// Affect the velocity by acceleration (specific to individual)
	pParticle->vVelocity += pParticle->vAcceleration * fTime;

	// Move the particle by velocity
	pParticle->vPosition += pParticle->vVelocity * fTime;

	/*
	// Bounce particle
	if( m_bBounceOnY && pParticle->vPosition.y < 0 )
	{
		pParticle->vPosition.y = 0;
		pParticle->vVelocity.y *= -m_fBounce;
	}
	*/

	// Compute distance
	//pParticle->fDistance = qV3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );

	// Decay color...
	if( m_iDecayDOP != DECAYDOP_NONE )
	{
		// !!!HERE!!! can use float multiply of unify::ColorUnit
		pParticle->color += pParticle->colDecay * fTime;
	}

	// Decay size
	if( m_iDecaySOP != DECAYSOP_NONE )
	{
		pParticle->fSize += pParticle->fSizeDecay	* fTime;
	}


	// Preform POP...
	POP* pPOP = m_pFirstPOP;
	while( pPOP )
	{
		// Loop through conditions...
		unify::Int32 iDoAction = 0;
		POPCONDITION* cond = pPOP->pFirstCond;
		while( cond && iDoAction != 2 )
		{
			switch( cond->Cond )
			{
			case POPCOND_IGNORE:
				break;

			case POPCOND_PASTDISTANCE:
				if( !pParticle ) break;
				if( !cond->pVar ) break;


				if( pParticle->fDistance > *cond->pVar->GeFloat() ) iDoAction = 1;
				else iDoAction = 2;
				break;
			
			case POPCOND_PLANE:
				if( !pParticle ) break;
				if( !cond->pVar ) break;
				{
					unify::Float f1, f2;
					f1 = qlib::TPlaneDotCoord( *cond->pVar->GetPLANE(), &pParticle->vPosition );
					f2 = qlib::TPlaneDotCoord( *cond->pVar->GetPLANE(), &pParticle->vLastPosition );

					if( (f1 < 0 && f2 > 0) || (f1 > 0 && f2 < 0) ) iDoAction = 1;
					else iDoAction = 2;
				}
				break;

			case POPCOND_PASTPLANE:
				if( !pParticle ) break;
				if( !cond->pVar ) break;
				{
					unify::Float f1, f2;
					f1 = qlib::TPlaneDotCoord( *cond->pVar->GetPLANE(), &pParticle->vPosition );
					f2 = qlib::TPlaneDotCoord( *cond->pVar->GetPLANE(), &pParticle->vBirthPosition );

					if( (f1 < 0 && f2 > 0) || (f1 > 0 && f2 < 0) ) iDoAction = 1;
					else iDoAction = 2;
				}
				break;

			case POPCOND_SLOWERTHAN:
				if( !pParticle ) break;
				if( !cond->pVar ) break;
				{
					unify::Float f, f2 = *cond->pVar->GeFloat();
					f = pParticle->vVelocity.Length();
					if( f < f2 )
					{
						iDoAction = 1;
					}
				}
				break;
			
			default:
				iDoAction = 2;
				break;
			}

			cond = cond->pNext;
		}

		if( iDoAction == 1 )
		{
			pParticle = PreformPOP( pParticle, pPOP );
		}

		// Move onto next
		pPOP = pPOP->pNext;
	}

	return pParticle;	// We SURVIVED!!! Yeah.
}

QParticle* QTemplate::PreformPOP( QParticle* pParticle, POP* pPOP )
{
	unify::Float f;

	// Loop through all actions...
	POPACTION* pAct = pPOP->pFirstAct;
	POPACTION* pNext;
	while( pAct )
	{
		pNext = pAct->pNext;

		switch( pAct->Act )
		{
		case POPACT_DIE:
			if( !pParticle ) break;

			KillParticle( pParticle );	// Giving parent could boost speed
			pParticle = 0;
			break;

		case POPACT_STOP:
			if( !pParticle ) break;

			// Reflect velocity...
			pParticle->vVelocity = unify::V3< unify::Float >( 0, 0, 0 );
			break;

		case POPACT_REFLECT:
			if( !pParticle ) break;
			
			// Reflect velocity...
			pParticle->vVelocity.x *= -1.0f;
			pParticle->vVelocity.y *= -1.0f;
			pParticle->vVelocity.z *= -1.0f;
			break;

		case POPACT_FIXDISTANCE:
			if( !pParticle ) break;

			/*
			if( pAct->dwState == PS_POINTER ) f = *(unify::Float*)pAct->pVoid;
			else f = pAct->fValue;
			*/
			f = *pAct->pVar->GeFloat();
			// Move particle to distance...
			qlib::V3Scale( &pParticle->vPosition, &pParticle->vPosition, (f / pParticle->fDistance) );
			pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			break;

		case POPACT_FIXPLANE:
			if( !pParticle ) break;

			qlib::PlaneIntersectLine( &pParticle->vPosition, pAct->pVar->GetPLANE(), &pParticle->vPosition, &pParticle->vLastPosition );
			pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			break;

		case POPACT_PLANEINTERSECT:
			{
				if( !pParticle ) break;

				// Try to intersect by LastPos...
				if( !qlib::PlaneIntersectLine( &pParticle->vPosition, pAct->pVar->GetPLANE(), &pParticle->vPosition, &pParticle->vLastPosition ) )
				{
					// Pos to LastPos doesn't intersect, try origin instead
					qlib::PlaneIntersectLine( &pParticle->vPosition, pAct->pVar->GetPLANE(), &pParticle->vPosition, &pParticle->vLastPosition );
				}
					
				pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			}
			break;

		case POPACT_GOTOPLANEORIGININTERSECT:
			if( !pParticle ) break;
			
			qlib::PlaneIntersectLine( &pParticle->vPosition, pAct->pVar->GetPLANE(), &pParticle->vPosition, &pParticle->vBirthPosition );
			pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			break;

		case POPACT_GOTOLAST:
			if( !pParticle ) break;

			pParticle->vPosition = pParticle->vLastPosition;
			pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			break;

		case POPACT_PLANEREFLECT:
			{
				if( !pAct->pVar ) break;
				if( !pParticle ) break;

				// Move to intersect point
				qlib::PlaneIntersectLine( &pParticle->vPosition, pAct->pVar->GetPLANE(), &pParticle->vPosition, &pParticle->vLastPosition );
				// (but more towards the last position)
				qlib::V3Lerp( &pParticle->vPosition, &pParticle->vPosition, &pParticle->vLastPosition, 0.5f );


				// Reflect velocity...
				pParticle->vVelocity.x *= -1;
				pParticle->vVelocity.y *= -1;
				pParticle->vVelocity.z *= -1;

				unify::Matrix mRot;
				mRot.RotationAxis( unify::V3< unify::Float >(pAct->pVar->GetPLANE()->a, pAct->pVar->GetPLANE()->b, pAct->pVar->GetPLANE()->c),
					D3DX_PI );

				mRot.TransformCoord( pParticle->vVelocity );

				
				pParticle->fDistance = qlib::V3Length( &(pParticle->vBirthPosition - pParticle->vPosition ) );
			}
			break;

		case POPACT_VELMUL:
			if( !pAct->pVar ) break;
			if( !pParticle ) break;


			// Multiply...
			pParticle->vVelocity.x *= *pAct->pVar->GeFloat();
			pParticle->vVelocity.y *= *pAct->pVar->GeFloat();
			pParticle->vVelocity.z *= *pAct->pVar->GeFloat();

			break;

		case POPACT_MOVEBYVELOCITY:
			{
				unify::Float f = 1.0f;
				if( !pParticle ) break;

				if( pAct->pVar ) f = *pAct->pVar->GeFloat();

				pParticle->vPosition.x += pParticle->vVelocity.x * f;
				pParticle->vPosition.y += pParticle->vVelocity.y * f;
				pParticle->vPosition.z += pParticle->vVelocity.z * f;
			}
			break;

		case POPACT_MOVEBY:
			{
				unify::Float f = 1.0f;
				if( !pParticle ) break;
				if( !pAct->pVar ) break;

				pParticle->vPosition.x += *pAct->pVar->GeFloat();
				pParticle->vPosition.y += *pAct->pVar->GeFloat();
				pParticle->vPosition.z += *pAct->pVar->GeFloat();
			}
			break;
		}

		pAct = pNext;
	}

	return pParticle;
}

/////
// QPSpray3D
/////

QPSpray3D::QPSpray3D()
{
	// Template List...
	m_pFirstTemplate	= 0;
	m_pLastTemplate		= 0;
	m_dwTemplateCount	= 0;

	// Emitter List...
	m_pFirstEmitter = 0;
	m_pLastEmitter = 0;
	m_dwEmitterCount = 0;

	m_bHandleEmitters = TRUE;
}

QPSpray3D::~QPSpray3D()
{
	ReleaseData();
	KillParticles();
	ReleaseEmitters();
}

// Release all emitters
void QPSpray3D::ReleaseEmitters()
{
	QEmitter* pEmitter = m_pFirstEmitter;
	QEmitter* pNext;

	while( pEmitter )
	{
		pNext = pEmitter->pNextEmitter;
		delete pEmitter;
		pEmitter = pNext;
	}

	m_pFirstEmitter = m_pLastEmitter = 0;

	m_dwEmitterCount = 0;
}

unify::Bool QPSpray3D::Process()
{
	HRESULT hr;

	// Don't process if we've just resumed
	if( System()->GetState( SY_RESUME ) ) return TRUE;

	static unify::Float fElapsed	= 0;
	if( fElapsed )
	{
		fElapsed += System()->Elapsed();
		fElapsed *= 0.5f;
	}
	else fElapsed = System()->Elapsed();

	unify::Float fNewElapsed;

	
	QParticle* pParticle;

	//if( !m_dwMaxParticles ) return S_OK;
	unify::DWord dwNumParticles;

	QTemplate* pTemplate;

	// Handle Emitters...
	QEmitter* pEmitter = m_pFirstEmitter;
	QEmitter* pParentEmitter = 0;
	QEmitter* pNextEmitter = 0;
	while( m_bHandleEmitters && pEmitter )
	{
		pNextEmitter = pEmitter->pNextEmitter;

		if( !pEmitter->bProcess )
		{
			pParentEmitter = pEmitter;	// We are now the parent
			pEmitter = pNextEmitter;	// Move to the next emitter
			continue;
		}


		fNewElapsed =  fElapsed + pEmitter->fElapsedLeft;	// Get new elapsed and add left over (from last)
		dwNumParticles = (unify::Int32)(fNewElapsed / pEmitter->fBirthRate);

		pEmitter->fElapsedLeft = fNewElapsed - (dwNumParticles * pEmitter->fBirthRate);

		pTemplate = pEmitter->pTemplate;

		unify::Float fAccelTime;
		unify::Matrix mRot;
		for( unify::DWord dwParticle = 0; dwParticle < dwNumParticles; dwParticle++ )
		{
			unify::V3< unify::Float > vPosition, vVelocity;
 			unify::Float fAngleX, fAngleY, fAngleZ;
			unify::Float fAngle = pEmitter->fAngle;
			unify::Float fAngleHalf = pEmitter->fAngle * 0.5f;
			unify::Float fVelocity;

			// Set position and velocity depending on emitter type
			switch( pEmitter->dwType )
			{
			case EMITTER_SPRAY:
				// Position...
				vPosition = pEmitter->vPosition;

				// Velocity...
				fAngleX = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleY = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleZ = (qlib::frandp() * fAngle) - fAngleHalf;

				qlib::MatrixRotationYawPitchRoll( &mRot, fAngleY, fAngleX, fAngleZ );
				qlib::V3TransformCoord( &vVelocity, &pEmitter->vDirection, &mRot );

				fVelocity = ((pEmitter->fMaxVelocity - pEmitter->fMinVelocity) * qlib::frandp()) + pEmitter->fMinVelocity;

				qlib::V3Scale( &vVelocity, &vVelocity, fVelocity );
				break;

			case EMITTER_OMNI:
				// Position...
				vPosition = pEmitter->vPosition;

				// Determine velocity
				fAngleX = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;
				fAngleY = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;
				fAngleZ = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;

				qlib::V3Normalize( &vVelocity, &unify::V3< unify::Float >( fAngleX, fAngleY, fAngleZ ) );

				fVelocity = ((pEmitter->fMaxVelocity - pEmitter->fMinVelocity) * qlib::frandp()) + pEmitter->fMinVelocity;

				qlib::V3Scale( &vVelocity, &vVelocity, fVelocity );
				break;

			case EMITTER_SPHERE:
				// Position...
				fAngleX = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;
				fAngleY = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;
				fAngleZ = (qlib::frandp() * D3DX_PI * 2) - D3DX_PI;
				qlib::V3Normalize( &vPosition, &unify::V3< unify::Float >( fAngleX, fAngleY, fAngleZ ) );

				qlib::V3Scale( &vPosition, &vPosition, (qlib::frandp() * (pEmitter->fRadius1 - pEmitter->fRadius2)) + pEmitter->fRadius2 );

				// Velocity...
				fAngleX = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleY = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleZ = (qlib::frandp() * fAngle) - fAngleHalf;

				qlib::MatrixRotationYawPitchRoll( &mRot, fAngleY, fAngleX, fAngleZ );
				qlib::V3TransformCoord( &vVelocity, &pEmitter->vDirection, &mRot );

				fVelocity = ((pEmitter->fMaxVelocity - pEmitter->fMinVelocity) * qlib::frandp()) + pEmitter->fMinVelocity;

				qlib::V3Scale( &vVelocity, &vVelocity, fVelocity );

				break;

			case EMITTER_BOX:
				// Position...
				vPosition.x = (qlib::frandp() * pEmitter->fWidth * 2) - pEmitter->fWidth;
				vPosition.y = (qlib::frandp() * pEmitter->fHeight * 2) - pEmitter->fHeight;
				vPosition.z = (qlib::frandp() * pEmitter->fDepth * 2) - pEmitter->fDepth;
				//qV3Normalize( &vPosition, &D3DXVECTOR3( fAngleX, fAngleY, fAngleZ ) );

				//qV3Scale( &vPosition, &vPosition, (qlib::frandp() * (pEmitter->fRadius1 - pEmitter->fRadius2)) + pEmitter->fRadius2 );

				// Velocity...
				fAngleX = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleY = (qlib::frandp() * fAngle) - fAngleHalf;
				fAngleZ = (qlib::frandp() * fAngle) - fAngleHalf;

				qlib::MatrixRotationYawPitchRoll( &mRot, fAngleY, fAngleX, fAngleZ );
				vVelocity = pEmitter->vDirection;
				mRot.TransformCoord( vVelocity );

				fVelocity = ((pEmitter->fMaxVelocity - pEmitter->fMinVelocity) * qlib::frandp()) + pEmitter->fMinVelocity;

				vVelocity *= fVelocity;

				break;
			}

			pParticle = pTemplate->SpawnParticle( pEmitter, &vPosition, &vVelocity );	// Also handles particle limits
			
			if( !pParticle ) break;

			fAccelTime = 0;
			fAccelTime = (dwNumParticles - (dwParticle + 1)) * pEmitter->fBirthRate;
			fAccelTime += pEmitter->fElapsedLeft;
			pTemplate->AgeParticle( pParticle,  fAccelTime );	// Returns particle or NULL
		}

		// Age, destroy and move to the next...
		pEmitter->fAge += fElapsed;
		if( pEmitter->fLifetime && (pEmitter->fAge >= pEmitter->fLifetime)  )
		{
			if( m_pLastEmitter == pEmitter )	// If we're the last, set our parent as last
			{
				m_pLastEmitter = pParentEmitter;
			}
			if( m_pFirstEmitter == pEmitter )	// If we are the first
			{
				m_pFirstEmitter = pNextEmitter;
			}
			if( pParentEmitter )
			{
				pParentEmitter->pNextEmitter = pEmitter->pNextEmitter;
			}

			delete pEmitter;
			m_dwEmitterCount--;
		}
		else
		{
			pParentEmitter = pEmitter;		// We are now the parent
		}
		pEmitter = pNextEmitter;
	}

	// Process Particles...
	// Loop through all templates...
	pTemplate = m_pFirstTemplate;
	while( pTemplate )
	{
		hr = pTemplate->Process();
		if( FAILED(hr) ) return FALSE;
		pTemplate = pTemplate->Next();
	}

	return TRUE;
}

/*
// Returns number of destroyed particles
unify::DWord QPSpray3D::ResizeSpray( unify::DWord dwMaxParticles )
{
	m_dwMaxParticles = dwMaxParticles;

	// Kill excess particles...
	QParticle* pParticle = m_pFirstParticle;
	QParticle* pNext;
	
	unify::DWord dwCount = 0;
	while( m_dwNumParticles > m_dwMaxParticles )
	{
		pNext = pParticle->pNext;
		m_pFirstParticle = pNext;
		
		// If we are the last particle, we are now empty
		if( m_pLastParticle == pParticle )
		{
			m_pLastParticle = 0;
		}

		delete pParticle;
		pParticle = pNext;
		m_dwNumParticles--;
		dwCount++;
	}

	return dwCount;
}
*/

/////
// POPVAR
/////

POPVAR::POPVAR( POPVAR* pvar )
{
	pVoid = 0;
	pvStore = pvar->pvStore;
	pvType = pvar->pvType;

	if( !pvar->pVoid )	return;
	
	switch( pvStore )
	{
	case PVSTORE_COPY:
		switch( pvType )
		{
		default:
			break;
		case PVTYPE_FLOAT:
			pVoid = (void*)new unify::Float( *(unify::Float*)pvar->pVoid );
			break;			
		case PVTYPE_DWORD:
			pVoid = (void*)new unify::DWord( *(unify::DWord*)pvar->pVoid );
			break;
		case PVTYPE_MATRIX:
			pVoid = (void*)new D3DXMATRIX( *(D3DXMATRIX*)pvar->pVoid );
			break;
		case PVTYPE_VECTOR3:
			pVoid = (void*)new D3DXVECTOR3( *(D3DXVECTOR3*)pvar->pVoid );
			break;
		case PVTYPE_PLANE:
			pVoid = (void*)new D3DXPLANE( *(D3DXPLANE*)pvar->pVoid );
			break;
		case PVTYPE_TEMPLATE:
			break;
		}
		break;

	case PVSTORE_POINTER:
		pVoid = pvar->pVoid;
		break;
	}
}

POPVAR::POPVAR( PVSTORE pvstore, PVTYPE pvtype, void* pvoid )
{
	pVoid = 0;
	pvStore = pvstore;
	pvType = pvtype;

	if( !pvoid )	return;
	
	switch( pvStore )
	{
	case PVSTORE_COPY:
		switch( pvType )
		{
		default:
			break;
		case PVTYPE_FLOAT:
			pVoid = (void*)new unify::Float( *(unify::Float*)pvoid );
			break;			
		case PVTYPE_DWORD:
			pVoid = (void*)new unify::DWord( *(unify::DWord*)pvoid );
			break;
		case PVTYPE_MATRIX:
			pVoid = (void*)new D3DXMATRIX( *(D3DXMATRIX*)pvoid );
			break;
		case PVTYPE_VECTOR3:
			pVoid = (void*)new D3DXVECTOR3( *(D3DXVECTOR3*)pvoid );
			break;
		case PVTYPE_PLANE:
			pVoid = (void*)new D3DXPLANE( *(D3DXPLANE*)pvoid );
			break;
		}
		break;

	case PVSTORE_POINTER:
		pVoid = pvoid;
		break;
	}
}

POPVAR::~POPVAR()
{
	if( !pVoid ) return;

	switch( pvStore )
	{
	case PVSTORE_COPY:
		/*
		switch( pvType )
		{
		case PVTYPE_FLOAT:
			delete pVoid;
			pVoid = 0;
			break;
		}
		*/
		delete pVoid;
		pVoid = 0;
		break;

	case PVSTORE_POINTER:	// Leave memory intact (just de-reference from it)
		pVoid = 0;
		break;
	}
}

void* POPVAR::GetVOIDP()
{
	return( pVoid );
}

unify::Float* POPVAR::GeFloat()
{
	return( (unify::Float*)pVoid );
}

unify::Int32* POPVAR::GeInt32()
{
	return( (unify::Int32*)pVoid );
}

unify::Matrix * POPVAR::GetMATRIX()
{
	return( (unify::Matrix *)pVoid );
}

unify::V3< unify::Float > * POPVAR::GetVECTOR3()
{
	return( (unify::V3< unify::Float > *)pVoid );
}

unify::Plane * POPVAR::GetPLANE()
{
	return( (unify::Plane *)pVoid );
}



/////
// POPCONDITION
/////

POPCONDITION::POPCONDITION( POPCOND cond, POPVAR* pvar )
{
	Cond = cond;
	dwState = 0;
	pVar = pvar;

	pNext = 0;
}

void POPCONDITION::SetVar( POPVAR* pvar )
{
	pVar = pvar;
}



/////
// POPACTION
/////

POPACTION::POPACTION( POPACT act , POPVAR* pvar )
{
	Act = act;
	dwState = 0;
	SetVar( pvar );

	pNext = 0;

}

void POPACTION::SetVar( POPVAR* pvar )
{
	//pVar = pvar;
	pVar = pvar;	//new POPVAR( pvar );	// Copy (get our own)
}



/////
// POP
/////

POP::POP()
{
	// Condition...
	pFirstCond = 0;
	pLastCond = 0;
	dwCondCount = 0;

	// Action...
	pFirstAct = 0;
	pLastAct = 0;
	dwActCount = 0;

	// Duration...
	Dur = POPDUR_IGNORE;
	dwDurState = 0;

	//A=B=C=D=E=F=0.0f;
	pNext = 0;
}

void POP::Reset()	// Allows us to be a sort of a template
{
	KillConditions();
	KillActions();

	Dur = POPDUR_IGNORE;
	dwDurState = 0;

	//A=B=C=D=E=F=0.0f;
	pNext = 0;
}

POPCONDITION* POP::AddCondition( POPCOND cond, POPVAR* pvar )
{
	POPCONDITION* pCond = new POPCONDITION( cond, pvar );
	if( !pCond ) return NULL;

	// Add to our condition list...
	if( pLastCond )
	{
		pLastCond->pNext = pCond;
	}
	else
	{
		pLastCond = pCond;
		pFirstCond = pCond;
	}

	dwCondCount++;
	return pCond;
}

POPACTION* POP::AddAction( POPACT act, POPVAR* pvar )
{
	POPACTION* pAct = new POPACTION( act, pvar );
	if( !pAct ) return NULL;

	// Add to our action list...
	if( pLastAct )
	{
		pLastAct->pNext = pAct;
	}
	else
	{
		pFirstAct = pAct;
	}
	pLastAct = pAct;

	dwActCount++;
	return pAct;
}

void POP::KillConditions()
{
	POPCONDITION* cond = pFirstCond;
	POPCONDITION* next;
	while( cond )
	{
		next = cond->pNext;
		delete cond;
		cond = next;
	}
	dwCondCount = 0;
	pFirstCond = 0;
	pLastCond = 0;
}

void POP::KillActions()
{
	POPACTION* act = pFirstAct;
	POPACTION* next;
	while( act )
	{
		next = act->pNext;
		delete act;
		act = next;
	}
	dwActCount = 0;
	pFirstAct = 0;
	pLastAct = 0;
}



/////
// QParticle
/////

QParticle::QParticle( QTemplate* pTemplate, QEmitter* pEmitter )
{
	pMyTemplate = pTemplate;
	pNext = 0;
	pMyEmitter = pEmitter;
	fDistance = 0;
}

unify::ColorUnit QParticle::GetColor()
{
	return color;
}



//////////
// QEmitter
//////////

QEmitter::QEmitter( unify::DWord dwEmitterType, QTemplate* pT )
{
	vDirection			= unify::V3< unify::Float >( 0, 0, 0 );
	fElapsedLeft		= 0;
	fLifetime			= 0;
	fAge				= 0;
	fMinVelocity		= 0;
	fMaxVelocity		= 0;
	pNextEmitter		= 0;
	bProcess			= TRUE;
	dwType				= dwEmitterType;

	fAngle				= D3DX_PI * 0.15f;
	fRadius1			= 0;
	fRadius2			= 0;
	fWidth				= 0;
	fHeight				= 0;
	fDepth				= 0;

	pTemplate			= pT;
}

QEmitter::~QEmitter()
{
}

void QEmitter::SetPosition( unify::V3< unify::Float > * pVec )
{
	vPosition = *pVec;
}

void QEmitter::SetDirection( unify::V3< unify::Float > * pVec )
{
	vDirection = *pVec;
	vDirection.Normalize();
}

void QEmitter::AddTime( unify::Float fTimeInSeconds )
{
	fAge += (fElapsedLeft += fTimeInSeconds);
}

void QEmitter::SetLifetime( unify::Float fTimeInSeconds )
{
	fLifetime = fTimeInSeconds;
}

void QEmitter::SetBirthRate( unify::Float fRate )
{
	fBirthRate = fRate;
}

void QEmitter::SetVelocity( unify::Float fMin, unify::Float fMax )
{
	fMinVelocity = fMin;
	if( !(fMaxVelocity = fMax) ) fMaxVelocity = fMin;
}

void QEmitter::SetAngle( unify::Float angle )
{
	fAngle = angle;
}

void QEmitter::SetRadius( unify::Float radius2, unify::Float radius1 )
{
	fRadius2 = radius2;
	if( !(fRadius1 = radius1) )
	{
		fRadius1 = fRadius2;
		fRadius2 = 0;
	}
}

void QEmitter::SetSize( unify::Float width, unify::Float height, unify::Float depth )
{
	fWidth	= width * 0.5f;
	fHeight = height * 0.5f;
	fDepth	= depth * 0.5f;
}

void QEmitter::Halt()
{
	bProcess = FALSE;
}

void QEmitter::Resume()
{
	bProcess = TRUE;
}



/////
// QTemplate
/////

unify::Bool QTemplate::Create( const unify::Char* pszName/*ignored*/, unify::DWord dwMaxParticles, unify::DWord dwParticleType )
{
	// Release current
	ReleaseData();
	KillParticles();

	m_dwMaxParticles = dwMaxParticles;

	m_dwParticleType = dwParticleType;

	return CreateData();	// Create VB and IB
}

unify::Bool QTemplate::CreateData()
{
	ReleaseData();

	// Create the vertex buffer...
	switch( m_dwParticleType )
	{
	case PARTICLE_TICK3D:
		m_VB.Create( m_dwMaxParticles * 6, 0, POINTVERTEX::FORMAT, VB_WRITEONLY );
		break;
	
	case PARTICLE_POINT:
		m_VB.Create( m_dwMaxParticles, 0, POINTVERTEX::FORMAT, VB_WRITEONLY | VB_POINTS );
		break;

	case PARTICLE_LINE:
		m_VB.Create( m_dwMaxParticles * 2, 0, POINTVERTEX::FORMAT, VB_WRITEONLY );
		break;
		
	case PARTICLE_BILLBOARD:
		m_VB.Create( m_dwMaxParticles * 4, m_dwMaxParticles * 6, COLORVERTEX::FORMAT, VB_WRITEONLY );
		break;

	case PARTICLE_BILLBOARD2:
		m_VB.Create( m_dwMaxParticles * 6, 0, COLORVERTEX::FORMAT, VB_WRITEONLY );
		break;

	case PARTICLE_GEOMETRY:
		break;

	case PARTICLE_PSPRITE:	// OLD DYNAMIC VERSION
		/*
		if( FAILED(GetDevice()->CreateVertexBuffer( m_dwDiscard * sizeof(POINTVERTEX),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
			D3DPOINTVERTEX::FORMAT, D3DPOOL_DEFAULT, &m_pVB, NULL )))
		{
			return FALSE;
		}
		*/
		//if( !m_VB.Create( m_dwDiscard, 
		
		break;

	case PARTICLE_POINTSPRITE:	// NEW NON-DYNAMIC VERSION (works better)
		m_VB.Create( m_dwMaxParticles, 0, POINTVERTEX::FORMAT, VB_WRITEONLY|VB_POINTS );
		break;

	case PARTICLE_PSIZE:	// Per-point sized particles
		m_VB.Create( m_dwMaxParticles, 0, PSIZEVERTEX::FORMAT, VB_WRITEONLY|VB_POINTS );
		break;

	case PARTICLE_TRIBOARD:	// Buggy
		/*
		if( FAILED(GetDevice()->CreateVertexBuffer( m_dwMaxParticles * (sizeof(QVertex) * 3),
			D3DUSAGE_WRITEONLY, QVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)) )
		{
			return FALSE;
		}
		*/
		m_VB.Create( m_dwMaxParticles * 3, 0, QVertex::FORMAT, VB_WRITEONLY );
		break;

	default:	// No vertex buffer needed
		break;
	}

	return TRUE;
}

void QTemplate::ReleaseData()
{
	m_VB.Release();
}

unify::DWord QTemplate::RenderedParticles()
{
	return( m_dwRenderedParticles );
}

void QTemplate::KillParticles()
{
	QParticle* pParticle = m_pFirstParticle;
	QParticle* pNext;

	while( pParticle )
	{
		pNext = pParticle->pNext;
		delete pParticle;
		pParticle = pNext;
	}

	m_pFirstParticle = 0;
	m_pLastParticle = 0;

	m_dwNumParticles = 0;
}

void QTemplate::IncrementEmitterReference()
{
	m_dwEmitterReferenceCount++;
}

void QTemplate::SetNext( QTemplate * pTemplate )
{
	m_pNext = pTemplate;
}

QTemplate * QTemplate::Next()
{
	return( m_pNext );
}

QParticle * QTemplate::SpawnParticle( QEmitter * pEmitter, unify::V3< unify::Float > * pPosition, unify::V3< unify::Float > * pVelocity )
{
	if( m_dwNumParticles >= m_dwMaxParticles )
	{ return NULL; }
	
	QParticle* pParticle = new QParticle( this, pEmitter );
	if( !pParticle ) return NULL;

	// Assign positions
	pParticle->vBirthPosition	= *pPosition;
	pParticle->vPosition		= *pPosition;
	pParticle->vLastPosition	= *pPosition;
	
	// Assign velocity
	pParticle->vVelocity		= *pVelocity;

	// Assgi acceleration
	pParticle->vAcceleration	= unify::V3< unify::Float >( 0, 0, 0 );

	// Determine age
	unify::Float pl;
	pl = qlib::irand( m_dwMinLifeTime, m_dwMaxLifeTime ) * 0.001f;	// SCALED TIME (x * 1000)
	pParticle->fAge = 0;
	pParticle->fLifeTime = pl;	// scale down

	// Determine our size
	unify::Float ps;
	ps = qlib::frandp() * (m_fMaxSize - m_fMinSize) + m_fMinSize;
	pParticle->fSize = ps;

	// Determine our color
	//Int32 iAlpha, iRed, iGreen, iBlue;
	unify::ColorUnit cuColor;
	switch( m_iBirthDOP )
	{
	case BIRTHDOP_NONE:
		cuColor = m_cuBirth1;
		break;

	case BIRTHDOP_FROMRANGE:
		cuColor = unify::ColorUnit::ARGB(
			(m_cuBirth2.a - m_cuBirth1.a) * qlib::frandp() + m_cuBirth1.a,
			(m_cuBirth2.r - m_cuBirth1.r) * qlib::frandp() + m_cuBirth1.r,
			(m_cuBirth2.g - m_cuBirth1.g) * qlib::frandp() + m_cuBirth1.g,
			(m_cuBirth2.b - m_cuBirth1.b) * qlib::frandp() + m_cuBirth1.b
			);
		/*
		iAlpha	= qlib::irand( m_iBirth_Alpha1,	m_iBirth_Alpha2 );
		iRed	= qlib::irand( m_iBirth_Red1,		m_iBirth_Red2 );
		iGreen	= qlib::irand( m_iBirth_Green1,	m_iBirth_Green2 );
		iBlue	= qlib::irand( m_iBirth_Blue1,	m_iBirth_Blue2 );
		*/
		break;

	case BIRTHDOP_LOWTOMAX:
		{
		unify::Float fPercent	= qlib::irand( m_iBirth_DifLow, 100 ) * 0.01f;	// qlib::frandp()
		/*
		iAlpha			= (unify::Int32)(m_iBirth_Alpha1 * fPercent);
		iRed			= (unify::Int32)(m_iBirth_Red1 * fPercent);
		iGreen			= (unify::Int32)(m_iBirth_Green1 * fPercent);
		iBlue			= (unify::Int32)(m_iBirth_Blue1 * fPercent);
		*/
		cuColor = m_cuBirth1 * fPercent;
		}
		break;

	}

	// Set the Diffuse Decay
	//Float fDecayAlpha, fDecayRed, fDecayGreen, fDecayBlue;
	unify::ColorUnit cuDecay;
	switch( m_iDecayDOP )
	{
	case DECAYDOP_NONE:
		cuDecay = unify::ColorUnit::Zero();
		break;

	case DECAYDOP_TOPERCENT:
		/*
		fDecayAlpha		= ((iAlpha	* m_fDecay_Alpha)	- iAlpha) / pl;
		fDecayRed		= ((iRed	* m_fDecay_Red)		- iRed)	/ pl;
		fDecayGreen		= ((iGreen	* m_fDecay_Green)	- iGreen) / pl;
		fDecayBlue		= ((iBlue	* m_fDecay_Blue)	- iBlue) / pl;
		*/
		break;

	case DECAYDOP_TOCOLOR:
		/*
		fDecayAlpha		= (m_fDecay_Alpha - iAlpha) / pl;
		fDecayRed		= (m_fDecay_Red - iRed)	/ pl;
		fDecayGreen		= (m_fDecay_Green - iGreen)	/ pl;
		fDecayBlue		= (m_fDecay_Blue - iBlue) / pl;
		*/
		break;

	}

	pParticle->color	= cuColor;
	pParticle->colDecay	= cuDecay;

	// Determine size decay
	switch( m_iDecaySOP )
	{
	case DECAYSOP_OVERAGE:
		pParticle->fSizeDecay = ((ps * m_fSizeDecay) - ps) / pl;
		break;

	case DECAYSOP_NONE:
	default:
		pParticle->fSizeDecay = 0;
		break;
	}

	// Add particle to stream...
	if( m_pLastParticle )
	{
		m_pLastParticle->pNext = pParticle;	// Assign us as the next particle to the last particle...
	}
	else	// no first particle (of no last)
	{
		m_pFirstParticle = pParticle;
	}

	m_pLastParticle = pParticle;	// We are the last particle
	m_dwNumParticles++;

	// Preform POP...
	POP* pPOP = m_pFirstPOP;
	while( pPOP )
	{
		// Loop through conditions...
		POPCONDITION* cond = pPOP->pFirstCond;
		unify::Int32 iDoAction = 0;	// 0 = not sure (don't), 1 = So far 'do', 2 = definetly don't
		while( cond )
		{
			if( cond->Cond == POPCOND_BIRTH ) iDoAction = 1;
			else if( cond->Cond != POPCOND_IGNORE )
			{
				iDoAction = 2;
				break;
			}

			cond = cond->pNext;
		}

		if( iDoAction == 1 )
		{
			pParticle = PreformPOP( pParticle, pPOP );
		}
		

		// Move onto next
		pPOP = pPOP->pNext;
	}

	return pParticle;
}

void QTemplate::SetTickSize( unify::Float fTickSize )
{
	m_fTickSize = fTickSize;
}

unify::Float QTemplate::TickSize()
{
	return( m_fTickSize );
}

void QTemplate::SetPointSize( unify::Float fPointSize_PS, unify::Float fPointSizeMin_PS )
{
	m_fPointSize_PS = fPointSize_PS;
	m_fPointSizeMin_PS = fPointSizeMin_PS;
}

void QTemplate::SetForce( unify::V3< unify::Float > * pVec )
{
	m_vForce = *pVec;
}

void QTemplate::SetLifetime( unify::Float fMin, unify::DWord fMax )
{
	m_dwMinLifeTime = (unify::DWord)(fMin * 1000); m_dwMaxLifeTime = (unify::DWord)(fMax * 1000);
	if( !m_dwMaxLifeTime ) m_dwMaxLifeTime = m_dwMinLifeTime;
}

void QTemplate::SetImage( QImage* pImage )
{
	m_pImage = pImage;
}

void QTemplate::SetBirthSize( unify::Float fMin, unify::Float fMax )
{
	m_fMinSize = fMin;
	if( !(m_fMaxSize=fMax) ) m_fMaxSize = fMin;
}

void QTemplate::SetBirthDOP( BirthDOPType iBirthDOP )
{
	m_iBirthDOP = iBirthDOP;
}

void QTemplate::SetDecayDOP( DecayDOPType iDecayDOP )
{
	m_iDecayDOP = iDecayDOP;
}

void QTemplate::SetDiffuse( unify::ColorUnit* pColor )
{
	m_cuBirth1 = *pColor;
	m_iBirthDOP		= BIRTHDOP_NONE;
}

void QTemplate::SetDiffuseRange( unify::ColorUnit* pColor1, unify::ColorUnit* pColor2 )
{

	m_cuBirth1	= *pColor1;
	m_cuBirth2	= *pColor2;

	m_iBirthDOP = BIRTHDOP_FROMRANGE;
}

void QTemplate::SetDecayDOPARGB( DecayDOPType iDOP , unify::ColorUnit* pDecay )
{
	m_iDecayDOP = iDOP;
	switch( iDOP )
	{
	case DECAYDOP_NONE:
		m_cuDecay = unify::ColorUnit::Black();
		break;

	case DECAYDOP_TOPERCENT:
	case DECAYDOP_TOCOLOR:
		m_cuDecay = *pDecay;
		break;
	}
}

void QTemplate::SetDiffuseLowToMax( unify::ColorUnit* pColor1, unify::Float fPercent )
{
	m_cuBirth1	= *pColor1;

	m_iBirth_DifLow = (unify::Int32)(fPercent * 100.0f);
	m_iBirthDOP = BIRTHDOP_LOWTOMAX;
}

void QTemplate::SetDecaySOP( DecaySOPType iSOP, unify::Float fSizeDecay )
{
	m_iDecaySOP = iSOP;
	m_fSizeDecay = fSizeDecay;
}

void QTemplate::SetParticleGeometry( GeometryPtr geometry )
{
	m_pParticleGeometry = geometry;
}

void QTemplate::SetParticleLength( unify::Float fLength )
{
	m_fLength = fLength;
}

void QTemplate::SetLineType( unify::DWord dwType )
{
	m_dwLineType = dwType;
}

POP* QTemplate::AddPOP()
{
	POP* pPOP = new POP;//( cond, state, var );
	if( !pPOP ) return NULL;

	if( m_pLastPOP )
	{
		m_pLastPOP->pNext = pPOP;
		m_pLastPOP = pPOP;
	}
	else
	{
		m_pLastPOP = pPOP;
		m_pFirstPOP = pPOP;
	}

	m_dwPOPCount++;
	return pPOP;
}

void QTemplate::KillPOPs()
{
	POP* pPOP = m_pFirstPOP;
	POP* pNext;
	
	while( pPOP )
	{
		pNext = pPOP->pNext;
		delete pPOP;
		pPOP = pNext;
	}

	m_pFirstPOP = 0;
	m_pLastPOP = 0;
	m_dwPOPCount = 0;
}

void QTemplate::SetBlend( Blend & blend )
{
	m_Blend = blend;
}



/////
// QPSpray3D
/////

void QPSpray3D::Render( RenderInfo & renderInfo )
{
	m_dwRenderedParticles = 0;

	//Loop through all templates...
	QTemplate * pTemplate = m_pFirstTemplate;
	while( pTemplate )
	{
		pTemplate->Render(); // TODO: Doesn't take a QRenderInfo?

		m_dwRenderedParticles += pTemplate->RenderedParticles();
		pTemplate = pTemplate->Next();
	}
}
