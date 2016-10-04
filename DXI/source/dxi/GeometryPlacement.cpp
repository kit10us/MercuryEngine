// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/GeometryPlacement.h>
#include <dxi/core/Game.h>

using namespace dxi;

GeometryPlacement::GeometryPlacement( Geometry::weak_ptr geometry )
: m_geometry( geometry )
{
}

GeometryPlacement::~GeometryPlacement()
{
	Destroy();
}

void GeometryPlacement::Create()
{
}

void GeometryPlacement::Destroy()
{
	m_objects.clear();
	m_geometry.reset();
}

void GeometryPlacement::Validate()
{
}

void GeometryPlacement::Invalidate()
{
}

void GeometryPlacement::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	/*
	TODO:

	// Render all geometry...
	unify::Matrix mWorldOriginal;
	unify::Matrix mWorldNew;

	win::DX::GetDxDevice()->GetTransform( D3DTS_WORLD, (D3DMATRIX*)&mWorldOriginal );
	mWorldNew = mWorldOriginal;

	//RenderInfo* pInfo
	for( std::vector< PlacementEntry >::const_iterator itr =  m_objects.begin(); itr != m_objects.end(); ++itr )
	{ 
		// Alter world matrix...
		unify::Matrix mTranslate;
		mTranslate.Translate( itr->vPos );

		mWorldNew = mTranslate * mWorldOriginal;
		win::DX::GetDxDevice()->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&mWorldNew );
		
		// Render geometry...
		m_geometry->Render( renderInfo, instanceData );
	}

	win::DX::GetDxDevice()->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&mWorldOriginal );
	*/
}

void GeometryPlacement::Update( unify::Seconds elapsed, GeometryInstanceData * instanceData )
{	
	// TODO: We need to keep seperate timing objects per each of these items.
	//m_geometry->Update( elapsed, instanceData );
}

void GeometryPlacement::AddPlace( const unify::V3< float > & position )
{
	m_BBox.AddBBoxWithPosition( m_geometry->GetBBox(), position );
	m_objects.push_back( PlacementEntry( position ) );
}

void GeometryPlacement::AddPlacesFromTexture( Texture::ptr texture, const unify::TexArea & area, float depth, unify::Color color )
{
	unify::Rect< long > rect;
	rect.left = (long)( texture->ImageSize().width * area.ul.u );
	rect.right = (long)( texture->ImageSize().width * area.dr.u );
	rect.top = (long)( texture->ImageSize().height * area.ul.v );
	rect.bottom = (long)( texture->ImageSize().height * area.dr.v );

	TextureLock lock;
	texture->LockRect( 0, lock, &rect, true );

	// Add the trees...
	unify::Color col;
	unify::V3< float > vPos;
	for( unsigned int v = 0; v < lock.uHeight; v++ )
	{
		for( unsigned int h = 0; h < lock.uWidth; h++ )
		{
			col = *((unify::Color *)((unsigned int*)lock.pBits + (lock.uStride * v) + (h) ));

			// TREE (standard)
			if( col == color )
			{
				vPos = unify::V3< float >( ((float)h / lock.uWidth) * 10.0f, depth, ((float)v / lock.uHeight) * 10.0f );
				vPos.x = vPos.x - 5;
				vPos.y;
				vPos.z = 5 - vPos.z;
				AddPlace( vPos );
			}
		}
	}

	texture->UnlockRect( 0 );
}

const unify::BBox< float > & GeometryPlacement::ComputeBounds()
{
	// TODO: nsure this feature is implemented.
	/*
	// Merge all bounds into our bounds...
	m_BBox.Initialize();
	Geometry* pGeo = m_GeoList.GotoFirst();
	while( pGeo )
	{
		m_BBox.AddBBox( pGeo->BoundingBox() );
		qRadiusAddRadius( &m_fBoundingRadius, pGeo->BoundingRadius() );
		pGeo = m_GeoList.GotoNext();
	}
	*/
	return m_BBox;
}