#include <Draw2D.h>
#include <TesterBase.h>
#include <assert.h>
#include <media/vertex/NonTextured.h>
#include <media/vertex/Textured.h>

void Draw2D::DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR colorUL, D3DCOLOR colorUR, D3DCOLOR colorDR, D3DCOLOR colorDL, D3DCOLOR innerUL, D3DCOLOR innerUR, D3DCOLOR innerDR, D3DCOLOR innerDL, float thickness )
{
	IDirect3DDevice9 & dxDevice = *displayActual.GetDxDevice();

	if ( thickness == 0 )
	{
		D3DXCOLOR innerXUL( innerUL );
		D3DXCOLOR innerXUR( innerUR );
		D3DXCOLOR innerXDL( innerDL );
		D3DXCOLOR innerXDR( innerDR );
		D3DXCOLOR center, centerU, centerD;
		D3DXColorLerp( &centerU, &innerXUL, &innerXUR, 0.5f );
		D3DXColorLerp( &centerD, &innerXDL, &innerXDR, 0.5f );
		D3DXColorLerp( &center, &centerU, &centerD, 0.5f );

   		media::vertex::NonTextured vb[] = 
		{
		    media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),
			
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),

			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),

			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center )
		};
    	dxDevice.SetFVF( media::vertex::NonTextured::FVF );
		dxDevice.SetTexture( 0, 0 );
		dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLELIST, 4, (void *)&vb[ 0 ], sizeof( media::vertex::NonTextured ) );
	}
	else
	{
		Rect rectInner( rect.ul.x + thickness, rect.ul.y + thickness, rect.dr.x - thickness, rect.dr.y - thickness );
		Rect & reIn = rectInner; // Abbreviated to simplify Vertex creation bellow...

  		media::vertex::NonTextured vb[] = 
		{
		    media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( reIn.ul.x, reIn.ul.y, innerUL ),
			
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( reIn.dr.x, reIn.ul.y, innerUR ),
			media::vertex::NonTextured( reIn.ul.x, reIn.ul.y, innerUL ),

			// Right
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( reIn.dr.x, reIn.ul.y, innerUR ),

			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( reIn.dr.x, reIn.dr.y, innerDR ),
			media::vertex::NonTextured( reIn.dr.x, reIn.ul.y, innerUR ),

			// Bottom
			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( reIn.dr.x, reIn.dr.y, innerDR ),

			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( reIn.ul.x, reIn.dr.y, innerDL ),
			media::vertex::NonTextured( reIn.dr.x, reIn.dr.y, innerDR ),

			// Left
			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( reIn.ul.x, reIn.dr.y, innerDL ),

			media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( reIn.ul.x, reIn.ul.y, innerUL ),
			media::vertex::NonTextured( reIn.ul.x, reIn.dr.y, innerDL )
		};
		dxDevice.SetFVF( media::vertex::NonTextured::FVF );
		dxDevice.SetTexture( 0, 0 );
		dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLELIST, 8, (void *)&vb[ 0 ], sizeof( media::vertex::NonTextured ) );
	}
}

void Draw2D::DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR colorUL, D3DCOLOR colorUR, D3DCOLOR colorDR, D3DCOLOR colorDL, float thickness )
{
	IDirect3DDevice9 & dxDevice = *displayActual.GetDxDevice();

	if ( thickness == 0 )
	{
		D3DXCOLOR colorXUL( colorUL );
		D3DXCOLOR colorXUR( colorUR );
		D3DXCOLOR colorXDL( colorDL );
		D3DXCOLOR colorXDR( colorDR );
		D3DXCOLOR center, centerU, centerD;
		D3DXColorLerp( &centerU, &colorXUL, &colorXUR, 0.5f );
		D3DXColorLerp( &centerD, &colorXDL, &colorXDR, 0.5f );
		D3DXColorLerp( &center, &centerU, &centerD, 0.5f );
		
		media::vertex::NonTextured vb[] = 
		{
			media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),
			
			media::vertex::NonTextured( rect.dr.x, rect.ul.y, colorUR ),
			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),

			media::vertex::NonTextured( rect.dr.x, rect.dr.y, colorDR ),
			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center ),

			media::vertex::NonTextured( rect.ul.x, rect.dr.y, colorDL ),
			media::vertex::NonTextured( rect.ul.x, rect.ul.y, colorUL ),
			media::vertex::NonTextured( rect.Center().x, rect.Center().y, center )
		};
		dxDevice.SetFVF( media::vertex::NonTextured::FVF );
		dxDevice.SetTexture( 0, 0 );
		dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLELIST, 4, (void *)&vb[ 0 ], sizeof( media::vertex::NonTextured ) );
	}
	else
	{
		D3DXCOLOR colorXUL( colorUL );
		D3DXCOLOR colorXUR( colorUR );
		D3DXCOLOR colorXDL( colorDL );
		D3DXCOLOR colorXDR( colorDR );
		D3DXCOLOR center, centerU, centerD;
		D3DXColorLerp( &centerU, &colorXUL, &colorXUR, 0.5f );
		D3DXColorLerp( &centerD, &colorXDL, &colorXDR, 0.5f );
		D3DXColorLerp( &center, &centerU, &centerD, 0.5f );
		Rect rectInner( rect.ul.x + thickness, rect.ul.y + thickness, rect.dr.x - thickness, rect.dr.y - thickness );
		Rect & reIn = rectInner; // Abbreviated to simplify Vertex creation bellow...

		struct Vertex
		{
			float x, y, z, rhw;
			D3DCOLOR diffuse;
		} vb[] = 
		{
			// Top
			{ rect.ul.x, rect.ul.y, 1,1, colorUL },
			{ rect.dr.x, rect.ul.y, 1,1, colorUR },
			{ reIn.ul.x, reIn.ul.y, 1,1, center },
			
			{ rect.dr.x, rect.ul.y, 1,1, colorUR },
			{ reIn.dr.x, reIn.ul.y, 1,1, center },
			{ reIn.ul.x, reIn.ul.y, 1,1, center },

			// Right
			{ rect.dr.x, rect.ul.y, 1,1, colorUR },
			{ rect.dr.x, rect.dr.y, 1,1, colorDR },
			{ reIn.dr.x, reIn.ul.y, 1,1, center },

			{ rect.dr.x, rect.dr.y, 1,1, colorDR },
			{ reIn.dr.x, reIn.dr.y, 1,1, center },
			{ reIn.dr.x, reIn.ul.y, 1,1, center },

			// Bottom
			{ rect.dr.x, rect.dr.y, 1,1, colorDR },
			{ rect.ul.x, rect.dr.y, 1,1, colorDL },
			{ reIn.dr.x, reIn.dr.y, 1,1, center },

			{ rect.ul.x, rect.dr.y, 1,1, colorDL },
			{ reIn.ul.x, reIn.dr.y, 1,1, center },
			{ reIn.dr.x, reIn.dr.y, 1,1, center },

			// Left
			{ rect.ul.x, rect.dr.y, 1,1, colorDL },
			{ rect.ul.x, rect.ul.y, 1,1, colorUL },
			{ reIn.ul.x, reIn.dr.y, 1,1, center },

			{ rect.ul.x, rect.ul.y, 1,1, colorUL },
			{ reIn.ul.x, reIn.ul.y, 1,1, center },
			{ reIn.ul.x, reIn.dr.y, 1,1, center },
		};
		unsigned int fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
		dxDevice.SetFVF( fvf );
		dxDevice.SetTexture( 0, 0 );
		dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLELIST, 8, (void *)&vb[ 0 ], sizeof( Vertex ) );
	}
}

// Inefficient, but we are going to use the above as it does a good enough job, and we don't care that much and optimal FPS (such a minor drop).
void Draw2D::DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR color )
{
	DrawRectangle( displayActual, rect, color, color, color, color, 0 );
}

void Draw2D::DrawLine( DisplayActual & displayActual, V2 pointA, V2 pointB, D3DCOLOR colorA, D3DCOLOR colorB, float thickness )
{
	if ( thickness == 0 )
	{
		return;
	}

	IDirect3DDevice9 & dxDevice = *displayActual.GetDxDevice();

	float angle = pointA.AngleInRadians( pointB ) + 3.141592f * 0.5f;

	V2 adjust( thickness * 0.5f * cos( angle ), thickness * 0.5f * sin( angle ) );
	V2 ul( pointA + adjust );
	V2 dl( pointA - adjust );
	V2 ur( pointB + adjust );
	V2 dr( pointB - adjust );

	struct Vertex
	{
		V2 pos; 
		float z, rhw;
		D3DCOLOR diffuse;
	} vb[] = 
	{
		{ ul, 1, 1, colorA },
		{ dl, 1, 1, colorA },
		{ ur, 1, 1, colorB },
		{ dr, 1, 1, colorB }
	};
	unsigned int fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	dxDevice.SetFVF( fvf );
	dxDevice.SetTexture( 0, 0 );
	dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (void *)&vb[ 0 ], sizeof( Vertex ) );
}

void Draw2D::DrawLine( DisplayActual & displayActual, V2 pointA, V2 pointB, D3DCOLOR colorA, float thickness )
{
	DrawLine( displayActual, pointA, pointB, colorA, colorA, thickness );
}