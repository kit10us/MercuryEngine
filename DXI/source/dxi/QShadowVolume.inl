inline HRESULT ShadowVolume::Render( IDirect3DDevice9 * pd3dDevice )
{
    pd3dDevice->SetFVF( D3DFVF_XYZ );

	return pd3dDevice->DrawPrimitiveUP( PrimitiveType::TriangleList, m_dwNumVertices/3,
                                        m_pVertices, sizeof(D3DXVECTOR3) );
}

inline HRESULT ShadowVolume::BuildFromMesh( LPD3DXMESH pMesh, D3DXVECTOR3 vLight )
{
    // Note: the MESHVERTEX format depends on the FVF of the mesh
    struct MESHVERTEX { D3DXVECTOR3 p, n; FLOAT tu, tv; };

    MESHVERTEX* pVertices;
    WORD*       pIndices;

    // Lock the geometry buffers
    pMesh->LockVertexBuffer( 0L, (LPVOID*)&pVertices );
    pMesh->LockIndexBuffer( 0L, (LPVOID*)&pIndices );
    DWORD dwNumFaces    = pMesh->GetNumFaces();

    // Allocate a temporary edge list
    WORD* pEdges = new WORD[dwNumFaces*6];
    if( pEdges == 0 )
    {
        pMesh->UnlockVertexBuffer();
        pMesh->UnlockIndexBuffer();
        return E_OUTOFMEMORY;
    }
    DWORD dwNumEdges = 0;

    // For each face
    for( DWORD i=0; i<dwNumFaces; i++ )
    {
        WORD wFace0 = pIndices[3*i+0];
        WORD wFace1 = pIndices[3*i+1];
        WORD wFace2 = pIndices[3*i+2];

        D3DXVECTOR3 v0 = pVertices[wFace0].p;
        D3DXVECTOR3 v1 = pVertices[wFace1].p;
        D3DXVECTOR3 v2 = pVertices[wFace2].p;

        // Transform vertices or transform light?
        D3DXVECTOR3 vCross1(v2-v1);
        D3DXVECTOR3 vCross2(v1-v0);
        D3DXVECTOR3 vNormal;
        D3DXVec3Cross( &vNormal, &vCross1, &vCross2 );

        if( D3DXVec3Dot( &vNormal, &vLight ) >= 0.0f )
        {
            AddEdge( pEdges, dwNumEdges, wFace0, wFace1 );
            AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
            AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
        }
    }

    for( i=0; i<dwNumEdges; i++ )
    {
        D3DXVECTOR3 v1 = pVertices[pEdges[2*i+0]].p;
        D3DXVECTOR3 v2 = pVertices[pEdges[2*i+1]].p;
        D3DXVECTOR3 unify::V3 = v1 - vLight*10;
        D3DXVECTOR3 unify::V4 = v2 - vLight*10;

        // Add a quad (two triangles) to the vertex list
        m_pVertices[m_dwNumVertices++] = v1;
        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = unify::V3;

        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = unify::V4;
        m_pVertices[m_dwNumVertices++] = unify::V3;
    }
    // Delete the temporary edge list
    delete[] pEdges;

    // Unlock the geometry buffers
    pMesh->UnlockVertexBuffer();
    pMesh->UnlockIndexBuffer();

    return S_OK;
}
