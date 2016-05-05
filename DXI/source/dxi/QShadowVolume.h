/*
 * Copyright (c) 2002 Quentin S. Smith
 * All Rights Reserved
 *
 * List of header files required for utilization of Q3D.
 * Also has the MEQ3D client for Mercury that allows more control over the environment.
 *		QEngine3D--->ME_CLIENT
 */
#pragma once

#include "dxi/Q3D.h"
#include "MEClient.h"
#include "dxi/Object.h"

namespace dxi
{
	class ShadowVolume
	{
		D3DXVECTOR3 m_pVertices[32000]; // Vertex data for rendering shadow volume
		DWORD       m_dwNumVertices;

	public:
		void Reset() { m_dwNumVertices = 0L; }
		HRESULT BuildFromMesh( ID3DXMesh * pObject, D3DXVECTOR3 vLight );
		HRESULT Render( IDirect3DDevice9 * pd3dDevice );
	};

	#include "dxi/QShadowVolume.inl"
}