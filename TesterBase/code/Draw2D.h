#pragma once

#include <pod.h>
#include <d3dx9.h>
#include <DisplayActual.h>

// Note, this is a "good enough" approach, quick and dirty. Mostly to keep it's logic "educational". There are so many ways to optimize this code,
// but that's not good training material, nor good maintainance.

class Draw2D
{
public:
	// A thickness of 0 means fill.
	// Note colors go in clockwise order from UL...
	static void DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR colorUL, D3DCOLOR colorUR, D3DCOLOR colorDR, D3DCOLOR colorDL, float thickness = 0 );

	static void DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR color );

	// A thickness of 0 means fill.
	// Note colors go in clockwise order from UL...
	static void DrawRectangle( DisplayActual & displayActual, Rect rect, D3DCOLOR colorUL, D3DCOLOR colorUR, D3DCOLOR colorDR, D3DCOLOR colorDL, D3DCOLOR innerUL, D3DCOLOR innerUR, D3DCOLOR innerDR, D3DCOLOR innerDL, float thickness = 0 );

	static void DrawLine( DisplayActual & displayActual, V2 pointA, V2 pointB, D3DCOLOR colorA, D3DCOLOR colorB, float thickness );
	
	static void DrawLine( DisplayActual & displayActual, V2 pointA, V2	 pointB, D3DCOLOR colorA, float thickness );

private:
};
