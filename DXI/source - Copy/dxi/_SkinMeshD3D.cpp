// Copyright (c) 2003 - 2011 Quentin S. Smith
// All Rights Reserved

#include <dxi\SkinMeshD3D.h>
#include <dxi/core/Win32Game.h>
#include "dxutil.h"

using namespace dxi;

//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//-----------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR Name, LPD3DXFRAME *ppNewFrame);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
    CAllocateHierarchy(SkinMeshD3D *pSkinMesh) :m_pSkinMesh(pSkinMesh){}

public:
    SkinMeshD3D* m_pSkinMesh;
};




//-----------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//-----------------------------------------------------------------------------
HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
    UINT cbLength;

    if (Name != NULL)
    {
        cbLength = lstrlen(Name) + 1;
        *pNewName = new char[cbLength];
        if (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy(*pNewName, Name, cbLength*sizeof(char));
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame( LPCTSTR Name, LPD3DXFRAME *ppNewFrame )
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;

    *ppNewFrame = NULL;

    pFrame = new D3DXFRAME_DERIVED;
    if (pFrame == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    hr = AllocateName( Name, (LPTSTR*)&pFrame->Name );
    if ( FAILED( hr ) )
	{
        goto e_Exit;
	}

    // initialize other data members of the frame
    D3DXMatrixIdentity( &pFrame->TransformationMatrix );
	pFrame->CombinedTransformationMatrix = unify::Matrix::MatrixIdentity();

    pFrame->pMeshContainer = 0;
    pFrame->pFrameSibling = 0;
    pFrame->pFrameFirstChild = 0;

    *ppNewFrame = pFrame;
    pFrame = 0;
e_Exit:
    delete pFrame;
    return hr;
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;

    LPD3DXMESH pMesh = NULL;

    *ppNewMeshContainer = NULL;

    // this sample does not handle patch meshes, so fail when one is found
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;

    // this sample does not FVF compatible meshes, so fail when one is found
    if (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, (LPTSTR*)&pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        

    NumFaces = pMesh->GetNumFaces();

    // if no normals are in the mesh, add them
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // clone the mesh to make room for the normals
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    core::Win32Game::GetGameInstance()->GetDxDevice(), &pMeshContainer->MeshData.pMesh );
        if (FAILED(hr))
            goto e_Exit;

        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;

        // now generate the normals for the pmesh
        D3DXComputeNormals( pMesh, NULL );
    }
    else  // if no normals, just add a reference to the mesh for the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }
        
    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials*2]; // additional space for normal maps
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // if materials provided, copy them
    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            char* strTexFn = (char*)pMeshContainer->pMaterials[iMaterial].pTextureFilename;

            if (strTexFn != NULL)
            {
                char strTexturePath[MAX_PATH];
                char strTextureTemp[MAX_PATH];
                DXUtil_FindMediaFileCb( strTexturePath, sizeof(strTexturePath), strTexFn );
                if( FAILED( D3DXCreateTextureFromFile( core::Win32Game::GetGameInstance()->GetDxDevice(), strTexturePath, 
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = 0;

				unify::Int32 i;
                for( i = 0; strTexFn[i] != '.' && strTexFn[i] != '\0' && (i+10) < MAX_PATH; i++)
				{
                    strTextureTemp[i] = strTexFn[i];
				}
				strTextureTemp[i] = '\0';
				
                strcat(strTextureTemp, "_bump.dds"); // Try to find a normal map

                DXUtil_FindMediaFileCb( strTexturePath, sizeof(strTexturePath), strTextureTemp );
                if( FAILED( D3DXCreateTextureFromFile( core::Win32Game::GetGameInstance()->GetDxDevice(), strTexturePath, 
                                                        &pMeshContainer->ppTextures[iMaterial + NumMaterials] ) ) )
                    pMeshContainer->ppTextures[iMaterial + NumMaterials] = 0;

                // don't remember a pointer into the dynamic memory, just forget the name after loading
                strTexFn = 0;
            }
        }
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = 0;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new TMatrix[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *((TMatrix*)pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }

        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
        hr = m_pSkinMesh->GenerateSkinnedMesh( pMeshContainer,m_pSkinMesh->getSkinningMethod() );
        if (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;
e_Exit:

    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
    SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}




//-----------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    SAFE_DELETE_ARRAY( pMeshContainer->Name );
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

    // release all the allocated textures
    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            SafeRelease( pMeshContainer->ppTextures[iMaterial] );
            SafeRelease( pMeshContainer->ppTextures[iMaterial + pMeshContainer->NumMaterials] );
        }
    }

    SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
    SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
    SafeRelease( pMeshContainer->pBoneCombinationBuf );
    SafeRelease( pMeshContainer->MeshData.pMesh );
    SafeRelease( pMeshContainer->pSkinInfo );
    SafeRelease( pMeshContainer->pOrigMesh );
    SAFE_DELETE( pMeshContainer );
    return S_OK;
}




SkinMeshD3D::SkinMeshD3D()
{
    m_pAnimController = NULL;
    m_pFrameRoot = NULL;

    m_SkinningMethod = D3DNONINDEXED;
    m_pBoneMatrices = NULL;
    m_uNumBoneMatricesMax = 0;
	
	m_pEffect = NULL;

	m_pszFilename = NullCreate();
	m_uNumBones = 0;
}

SkinMeshD3D::SkinMeshD3D( const TChar* pszFilename )
{
    m_pAnimController = NULL;
    m_pFrameRoot = NULL;

    m_SkinningMethod = D3DNONINDEXED;
    m_pBoneMatrices = NULL;
    m_uNumBoneMatricesMax = 0;

	m_pEffect = NULL;

	LoadSkinnedMesh( pszFilename );
}

SkinMeshD3D::~SkinMeshD3D()
{
	NullDelete( m_pszFilename );
	DeleteDeviceObjects();
}

//-----------------------------------------------------------------------------
// Name: GenerateSkinnedMesh()
// Desc: Called either by CreateMeshContainer when loading a skin mesh, or when 
//       changing methods.  This function uses the pSkinInfo of the mesh 
//       container to generate the desired drawable mesh and bone combination 
//       table.
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer,
	                                      METHOD                     SkinningMethod)
{
    HRESULT hr = S_OK;

    m_SkinningMethod = SkinningMethod;
    if (pMeshContainer->pSkinInfo == NULL)
        return hr;

    SafeRelease( pMeshContainer->MeshData.pMesh );
    SafeRelease( pMeshContainer->pBoneCombinationBuf );

    // if non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
    if (m_SkinningMethod == D3DNONINDEXED)
    {

        hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
                                   (
                                       pMeshContainer->pOrigMesh,
                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
                                       pMeshContainer->pAdjacency, 
                                       NULL, NULL, NULL, 
                                       &pMeshContainer->NumInfl,
                                       &pMeshContainer->NumAttributeGroups, 
                                       &pMeshContainer->pBoneCombinationBuf, 
                                       &pMeshContainer->MeshData.pMesh
                                   );
        if (FAILED(hr))
            goto e_Exit;


        /* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
           Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
           drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
        LPD3DXBONECOMBINATION rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

        // look for any set of bone combinations that do not fit the caps
        for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups; pMeshContainer->iAttributeSW++)
        {
            DWORD cInfl   = 0;

            for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
            {
                if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
                {
                    ++cInfl;
                }
            }

            if (cInfl > GetDisplay()->GetCaps()->MaxVertexBlendMatrices)
            {
                break;
            }
        }

        // if there is both HW and SW, add the Software Processing flag
        if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
        {
            LPD3DXMESH pMeshTmp;

            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING|pMeshContainer->MeshData.pMesh->GetOptions(), 
                                                pMeshContainer->MeshData.pMesh->GetFVF(),
                                                core::Win32Game::GetGameInstance()->GetDxDevice(), &pMeshTmp);
            if (FAILED(hr))
            {
                goto e_Exit;
            }

            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMeshTmp;
            pMeshTmp = NULL;
        }
    }
    // if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if (m_SkinningMethod == D3DINDEXED)
    {
        DWORD NumMaxFaceInfl;
        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl);
        pIB->Release();
        if (FAILED(hr))
            goto e_Exit;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

        if (GetDisplay()->GetCaps()->MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
        {
            // HW does not support indexed vertex blending. Use SW instead
            pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            // using hardware - determine palette size from caps and number of bones
            // If normals are present in the vertex data that needs to be blended for lighting, then 
            // the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
            pMeshContainer->NumPaletteEntries = min( ( GetDisplay()->GetCaps()->MaxVertexBlendMatrixIndex + 1 ) / 2, 
                                                     pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL, 
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;
    }
    // if vertex shader indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if ((m_SkinningMethod == D3DINDEXEDVS) || (m_SkinningMethod == D3DINDEXEDHLSLVS))
    {
        // Get palette size
        // First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
        // (96 - 9) /3 i.e. Maximum constant count - used constants 
        UINT MaxMatrices = 26; 
        pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
        if (GetDisplay()->GetCaps()->VertexShaderVersion >= D3DVS_VERSION(1, 1))
        {
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }
        else
        {
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }

        LPD3DXMESH pOrigMesh = pMeshContainer->pOrigMesh;

        // Create U, V, and UxV* vectors for textures space normal mapping
        // Now the app is a few fps faster.
        // * In MS terms: tangent, binormal, and normal
        D3DVERTEXELEMENT9 veDecl[MAX_FVF_DECL_SIZE], *pveDecl; 
        D3DVERTEXELEMENT9 veDeclTan[] = 
        {
	        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	        { 0, 12, D3DDECLTYPE_FLOAT1,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	        { 0, 16, D3DDECLTYPE_UBYTE4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	        { 0, 20, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	        { 0, 32, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	        { 0, 40, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
	        { 0, 52, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
	        D3DDECL_END()
        };

        D3DVERTEXELEMENT9 veDeclNorm[] = 
        {
	        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	        { 0, 12, D3DDECLTYPE_FLOAT1,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
	        { 0, 16, D3DDECLTYPE_UBYTE4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
	        { 0, 20, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
	        { 0, 32, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	        D3DDECL_END()
        };
        pOrigMesh->GetDeclaration(veDecl);

        bool cmp;
        if(m_SkinningMethod == D3DINDEXEDVS)
        {
            pveDecl = veDeclNorm;
            cmp = (memcmp(veDecl, veDeclNorm, sizeof(veDeclNorm)) != 0);
        }
        else
        {
            pveDecl = veDeclTan;
            cmp = (memcmp(veDecl, veDeclTan, sizeof(veDeclTan)) != 0);
        }

        if(cmp)
        {
            LPD3DXMESH pMesh;
            hr = pOrigMesh->CloneMesh(pOrigMesh->GetOptions(), pveDecl, core::Win32Game::GetGameInstance()->GetDxDevice(), &pMesh);
            if (!FAILED(hr))
            {
                //pOrigMesh->Release();
                pOrigMesh = pMesh;

                // Tangents can only be generated before the IndexedBlended conversion
                hr = D3DXComputeTangent( pOrigMesh, 0, 0, 0, TRUE, NULL); // This is a usefull function

                pMesh = NULL;
            }
        }

        SafeRelease(pMeshContainer->MeshData.pMesh);

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL,             
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        pOrigMesh->Release();

        D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
        LPD3DVERTEXELEMENT9 pDeclCur;
        hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
        if (FAILED(hr))
            goto e_Exit;

        // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
        //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
        //          this is more of a "cast" operation
        pDeclCur = pDecl;
        while (pDeclCur->Stream != 0xff)
        {
            if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
                pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
            pDeclCur++;
        }

        hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
        if (FAILED(hr))
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if (m_uNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
        {
            m_uNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            SafeDeleteArray( m_pBoneMatrices ); 
            m_pBoneMatrices  = new TMatrix[m_uNumBoneMatricesMax];
            if( m_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }

    }
    // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    else if (m_SkinningMethod == SOFTWARE)
    {
        hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
                                              core::Win32Game::GetGameInstance()->GetDxDevice(), &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
        if (FAILED(hr))
            goto e_Exit;

        delete[] pMeshContainer->pAttributeTable;
        pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
        if (pMeshContainer->pAttributeTable == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
        if (FAILED(hr))
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if (m_uNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
        {
            m_uNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            SafeDeleteArray( m_pBoneMatrices ) 
            m_pBoneMatrices = new TMatrix[m_uNumBoneMatricesMax];
            if( m_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }
    }
    else  // invalid m_SkinningMethod value
    {        
        // return failure due to invalid skinning method value
        hr = E_INVALIDARG;
        goto e_Exit;
    }

e_Exit:
    return hr;
}




//-----------------------------------------------------------------------------
// Name: DrawMeshContainer()
// Desc: Called to render a mesh in the hierarchy
//-----------------------------------------------------------------------------
void SkinMeshD3D::DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase,
					                 const TMatrix* pmatView )
{
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    UINT NumBlend;
    UINT iAttrib;
    DWORD AttribIdPrev;
    LPD3DXBONECOMBINATION pBoneComb;

    UINT iMatrixIndex;
    UINT iPaletteEntry;
    TMatrix matTemp;

    // first check for skinning
    if( pMeshContainer->pSkinInfo != NULL )
    {
        if( m_SkinningMethod == D3DNONINDEXED )
        {
            AttribIdPrev = UNUSED32; 
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

            // Draw using default vtx processing of the device (typically HW)
            for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++ )
            {
                NumBlend = 0;
				UInt32 u;
                for( u = 0; u < pMeshContainer->NumInfl; ++u )
                {
                    if( pBoneComb[iAttrib].BoneId[u] != UINT_MAX )
                    {
                        NumBlend = u;
                    }
                }

                if( GetDisplay()->GetCaps()->MaxVertexBlendMatrices >= NumBlend + 1 )
                {
                    // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                    for( u = 0; u < pMeshContainer->NumInfl; ++u )
                    {
                        iMatrixIndex = pBoneComb[iAttrib].BoneId[u];
                        if( iMatrixIndex != UINT_MAX )
                        {
                            qMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
	                        //D3DXMatrixMultiplyTranspose(&matTemp, &matTemp, pmatView);
							qMatrixMultiply( &matTemp, &matTemp, &m_mWorld );
							GetDisplay()->SetTransform( Transform::WorldNum( u ), &matTemp );
                        }
                    }

                    // lookup the material used for this subset of faces
                    if( (AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32) )
                    {
						// !!!HERE!!! Need to switch this to use the effect (UseEffect)
                        core::Win32Game::GetGameInstance()->GetDxDevice()->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                        core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                        AttribIdPrev = pBoneComb[iAttrib].AttribId;
                    }

					// Prepare the transformation matrices...
					GetDisplay()->PrepareFixedFunctionTransform( NumBlend );

                    // draw the subset now that the correct material and matrices are loaded
                    pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
                }
            }

            // If necessary, draw parts that HW could not handle using SW
            if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
            {
                AttribIdPrev = UNUSED32; 
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(TRUE);
                for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
                {
                    NumBlend = 0;
                    for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                    {
                        if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
                        {
                            NumBlend = i;
                        }
                    }

                    if (GetDisplay()->GetCaps()->MaxVertexBlendMatrices < NumBlend + 1)
                    {
                        // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                        for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                        {
                            iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                            if (iMatrixIndex != UINT_MAX)
                            {
                                qMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
								GetDisplay()->SetTransform( Transform::WorldNum( i ), &matTemp );
                            }
                        }

                        core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

                        // lookup the material used for this subset of faces
                        if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
                        {
                            core::Win32Game::GetGameInstance()->GetDxDevice()->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                            core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                            AttribIdPrev = pBoneComb[iAttrib].AttribId;
                        }

                        // draw the subset now that the correct material and matrices are loaded
                        pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
                    }
                }
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(FALSE);
            }

            core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_VERTEXBLEND, 0);
        }
        else if (m_SkinningMethod == D3DINDEXED)
        {
            // if hw doesn't support indexed vertex processing, switch to software vertex processing
            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(TRUE);
            }

            // set the number of vertex blend indices to be blended
            if (pMeshContainer->NumInfl == 1)
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
            else
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1);

            if (pMeshContainer->NumInfl)
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

            // for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
                // first calculate all the world matrices
                for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if (iMatrixIndex != UINT_MAX)
                    {
                        qMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
						GetDisplay()->SetTransform( Transform::WorldNum( iPaletteEntry ), &matTemp );
                    }
                }
                
                // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );

                // finally draw the subset with the current world matrix palette and material state
                pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
            }

            // reset blending state
            core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
            core::Win32Game::GetGameInstance()->GetDxDevice()->SetRenderState(D3DRS_VERTEXBLEND, 0);

            // remember to reset back to hw vertex processing if software was required
            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(FALSE);
            }
        }
        else if (m_SkinningMethod == D3DINDEXEDVS) 
        {
            // Use COLOR instead of UBYTE4 since Geforce3 does not support it
            // vConst.w should be 3, but due to COLOR/UBYTE4 issue, mul by 255 and add epsilon
            D3DXVECTOR4 vConst( 1.0f, 0.0f, 0.0f, 765.01f );

            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(TRUE);
            }

            //core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShader(m_pIndexedVertexShader[pMeshContainer->NumInfl - 1]);

            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
                // first calculate all the world matrices
                for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if (iMatrixIndex != UINT_MAX)
                    {
                        qMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
                        qMatrixMultiplyTranspose(&matTemp, &matTemp, pmatView);
                        core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShaderConstantF(iPaletteEntry*3 + 9, (float*)&matTemp, 3);
                    }
                }

                // Sum of all ambient and emissive contribution
                D3DXCOLOR color1(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient);
                D3DXCOLOR color2(.25, .25, .25, 1.0);
                D3DXCOLOR ambEmm;
                D3DXColorModulate(&ambEmm, &color1, &color2);
                ambEmm += D3DXCOLOR(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Emissive);

                // set material color properties 
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShaderConstantF(8, (float*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse), 1);
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShaderConstantF(7, (float*)&ambEmm, 1);
                vConst.y = pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Power;
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShaderConstantF(0, (float*)&vConst, 1);

                core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);

                // finally draw the subset with the current world matrix palette and material state
                pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
                
            }

            // remember to reset back to hw vertex processing if software was required
            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(FALSE);
            }
            core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShader(NULL);

        }
		/*
        else if (m_SkinningMethod == D3DINDEXEDHLSLVS) 
        {
            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(TRUE);
            }

            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            { 
                // first calculate all the world matrices
                for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if (iMatrixIndex != UINT_MAX)
                    {
                        qMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
                        qMatrixMultiply(&m_pBoneMatrices[iPaletteEntry], &matTemp, pmatView);
                    }
                }
                m_pEffect->SetMatrixArray( "mWorldMatrixArray", m_pBoneMatrices, pMeshContainer->NumPaletteEntries );

                // Sum of all ambient and emissive contribution
                D3DXCOLOR color1(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient);
                D3DXCOLOR color2(.25, .25, .25, 1.0);
                D3DXCOLOR ambEmm;
                D3DXColorModulate(&ambEmm, &color1, &color2);
                ambEmm += D3DXCOLOR(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Emissive);

                // set material color properties 
                //m_pEffect->SetVector("cDiffuse", (D3DXVECTOR4*)&(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse));
                //m_pEffect->SetVector("cAmbient", (D3DXVECTOR4*)&ambEmm);

                // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
                //core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                m_pEffect->SetTexture( "tDiffuse", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                m_pEffect->SetTexture( "tNormal", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId + pMeshContainer->NumMaterials] );

                // Set CurNumBones to select the correct vertex shader for the number of bones
                m_pEffect->SetInt( "CurNumBones", pMeshContainer->NumInfl -1);

                // Start the effect now all parameters have been updated
                UINT numPasses;
                m_pEffect->Begin( &numPasses, D3DXFX_DONOTSAVESTATE );
                for( UINT iPass = 0; iPass < numPasses; iPass++ )
                {
                    m_pEffect->Pass( iPass );
                    // draw the subset with the current world matrix palette and material state
                    pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
                }

                m_pEffect->End();        

                core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShader(NULL);
            }

            // remember to reset back to hw vertex processing if software was required
            if (pMeshContainer->UseSoftwareVP)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetSoftwareVertexProcessing(FALSE);
            }
        }
		*/
        else if (m_SkinningMethod == SOFTWARE)
        {
            TMatrixIdentity		Identity;
            DWORD       cBones  = pMeshContainer->pSkinInfo->GetNumBones();
            DWORD       iBone;
            PBYTE       pbVerticesSrc;
            PBYTE       pbVerticesDest;

            // set up bone transforms
            for (iBone = 0; iBone < cBones; ++iBone)
            {
                qMatrixMultiply
                (
                    &m_pBoneMatrices[iBone],                 // output
                    &pMeshContainer->pBoneOffsetMatrices[iBone], 
                    pMeshContainer->ppBoneMatrixPtrs[iBone]
                );
            }

            // set world transform
			GetDisplay()->SetTransform( Transform::World, &Identity);

            pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
            pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);

            // generate skinned mesh
            pMeshContainer->pSkinInfo->UpdateSkinnedMesh( (D3DXMATRIX*)m_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest );

            pMeshContainer->pOrigMesh->UnlockVertexBuffer();
            pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

            for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetMaterial(&(pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
                core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
                pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
            }
        }
        else // bug out as unsupported mode
        {
            return;
        }
    }

    
	
	else  // standard mesh, just draw it after setting material properties
    {
		TMatrix mTemp;
		qMatrixMultiply( &mTemp, (TMatrix*)&pFrame->CombinedTransformationMatrix, &m_mWorld );
		GetDisplay()->SetTransform( Transform::World, &mTemp );

        //core::Win32Game::GetGameInstance()->GetDxDevice()->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            core::Win32Game::GetGameInstance()->GetDxDevice()->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
            core::Win32Game::GetGameInstance()->GetDxDevice()->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
            pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
        }
    }
}

//-----------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: Called to render a frame in the hierarchy
//-----------------------------------------------------------------------------
void SkinMeshD3D::DrawFrame(LPD3DXFRAME pFrame,
					const TMatrix* pmatView)
{
    LPD3DXMESHCONTAINER pMeshContainer;

    pMeshContainer = pFrame->pMeshContainer;
    while (pMeshContainer != NULL)
    {
        DrawMeshContainer(pMeshContainer, pFrame, pmatView);

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        DrawFrame(pFrame->pFrameSibling, pmatView);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame(pFrame->pFrameFirstChild, pmatView);
    }
}
//-----------------------------------------------------------------------------
// Name: SetupBoneMatrixPointersOnMesh()
// Desc: Called to setup the pointers for a given bone to its transformation matrix
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    // if there is a skinmesh, then setup the bone matrices
    if (pMeshContainer->pSkinInfo != NULL)
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();
		m_uNumBones += cBones;	// possibly just (=)

        pMeshContainer->ppBoneMatrixPtrs = new TMatrix*[cBones];
        if (pMeshContainer->ppBoneMatrixPtrs == NULL)
            return E_OUTOFMEMORY;

        for (iBone = 0; iBone < cBones; iBone++)
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
            if (pFrame == NULL)
                return E_FAIL;

            pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
        }
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SetupBoneMatrixPointers()
// Desc: Called to setup the pointers for a given bone to its transformation matrix
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
    HRESULT hr;

    if (pFrame->pMeshContainer != NULL)
    {
        hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
        if (FAILED(hr))
            return hr;
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}

void SkinMeshD3D::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, const TMatrix* pParentMatrix )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    if( pParentMatrix != NULL )
	{
        qMatrixMultiply( &pFrame->CombinedTransformationMatrix, (TMatrix*)&pFrame->TransformationMatrix, pParentMatrix );
	}
    else
	{
        pFrame->CombinedTransformationMatrix = *((TMatrix*)&pFrame->TransformationMatrix);
	}

    if (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}

void SkinMeshD3D::UpdateSkinningMethod(LPD3DXFRAME pFrameBase,
							            METHOD            SkinningMethod)
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer;

    m_SkinningMethod = SkinningMethod;
    pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;

    while (pMeshContainer != NULL)
    {
        GenerateSkinnedMesh(pMeshContainer, m_SkinningMethod);

        pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
    }

    if (pFrame->pFrameSibling != NULL)
    {
        UpdateSkinningMethod(pFrame->pFrameSibling, m_SkinningMethod);
    }

    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateSkinningMethod(pFrame->pFrameFirstChild, m_SkinningMethod);
    }
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: This creates all device-dependent managed objects, such as managed
//       textures and managed vertex buffers.
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::InitDeviceObjects( const char*            strMeshPath,
										char*            strSkinnedMeshFXPath,							
										D3DXVECTOR3*      vObjectCenter,
										FLOAT*            fObjectRadius)
{
    HRESULT    hr;
    CAllocateHierarchy Alloc(this);

	m_uNumBones = 0;

    hr = D3DXLoadMeshHierarchyFromX( strMeshPath, D3DXMESH_MANAGED, core::Win32Game::GetGameInstance()->GetDxDevice(), &Alloc, NULL, &m_pFrameRoot, &m_pAnimController );
    if (FAILED(hr))
        return hr;

	NullCopy( m_pszFilename, strMeshPath );

    hr = SetupBoneMatrixPointers(m_pFrameRoot);
    if (FAILED(hr))
        return hr;

    hr = D3DXFrameCalculateBoundingSphere(m_pFrameRoot, vObjectCenter, fObjectRadius);
    if (FAILED(hr))
        return hr;

    // Create Effect for HLSL skinning
	if( strSkinnedMeshFXPath )
	{
		hr = D3DXCreateEffectFromFile( core::Win32Game::GetGameInstance()->GetDxDevice(), strSkinnedMeshFXPath, NULL,
										   NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, NULL);
	    if (FAILED(hr))
		    return hr;
	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restore device-memory objects and state after a device is created or
//       resized.
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::RestoreDeviceObjects()
{
    //HRESULT hr;

    // Restore Effect
    if ( m_pEffect )
        m_pEffect->OnResetDevice();

	/*
    // load the indexed vertex shaders
    for (DWORD iInfl = 0; iInfl < 4; ++iInfl)
    {
        LPD3DXBUFFER pCode;

        // Assemble the vertex shader file
        if( FAILED( hr = D3DXAssembleShaderFromResource(NULL, MAKEINTRESOURCE(IDD_SHADER1 + iInfl), NULL, NULL, 0, &pCode, NULL ) ) )
            return hr;

        // Create the vertex shader
        if( FAILED( hr = core::Win32Game::GetGameInstance()->GetDxDevice()->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),
                                                           &m_pIndexedVertexShader[iInfl] ) ) )
        {
            return hr;
        }

        pCode->Release();
    }
	*/

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::InvalidateDeviceObjects()
{

	/*
    // release the vertex shaders
    for (DWORD iInfl = 0; iInfl < 4; ++iInfl)
    {
        SafeRelease(m_pIndexedVertexShader[iInfl]);
    }
	*/

    // Invalidate Effect
    if ( m_pEffect )
        m_pEffect->OnLostDevice();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT SkinMeshD3D::DeleteDeviceObjects()
{
    CAllocateHierarchy Alloc(this);

    D3DXFrameDestroy(m_pFrameRoot, &Alloc);

    SafeRelease(m_pAnimController);

    SafeRelease(m_pEffect);

    return S_OK;
}



//////////////////////////
//////////////////////////
Bool SkinMeshD3D::LoadSkinnedMesh( const TChar* pszFilename )
{
	Float fRadius = 10.0f;	// ?
	HRESULT hr;
	hr = InitDeviceObjects( pszFilename, NULL, &D3DXVECTOR3(0,0,0), &fRadius );
	ComputeBounds();
	return SUCCEEDED(hr);
}

// Frame version...
void SkinMeshD3D::Render( RenderInfo* pInfo )
{
	GetDisplay()->GetTransform( TransformWorld0, &m_mWorld );
	
	TMatrix mView;
	GetDisplay()->GetTransform( TransformView, &mView );
	
	TMatrix mProjection;
	GetDisplay()->GetTransform( TransformProjection, &mProjection );

	//if( m_pEffect )
	//{
	//	m_pEffect->SetMatrix( "mViewProj", pmatProj);
	//	m_pEffect->SetVector( "lhtDir", &vLightDir);
	//}

    // set the projection matrix for the vertex shader based skinning method
    //D3DXMATRIXA16 matProj; 
    //D3DXMatrixTranspose(&matProj, pmatProj);
    //core::Win32Game::GetGameInstance()->GetDxDevice()->SetVertexShaderConstantF(2, (float*)&matProj, 4);

	DrawFrame( m_pFrameRoot, &mView );

	//return SUCCEEDED( hr );
}

void SkinMeshD3D::SubComputeBounds( const D3DXFRAME* pFrame )
{
	if( pFrame->pMeshContainer )
	{
		ID3DXMesh* pMesh = pFrame->pMeshContainer->MeshData.pMesh;

		TDWord dwFVF = pMesh->GetFVF();
		UInt32 uSizeOfFVF = qSizeOfFVF( dwFVF );

		TByte*	pVertices;

		// Lock the geometry buffers
		pMesh->LockVertexBuffer( 0L, (void**)&pVertices );
		TDWord dwNumVertices = pMesh->GetNumVertices();

		Float fNewBounds = 0.0f;
		TV3 vVec;
		for( TDWord i = 0; i < dwNumVertices; i++ )
		{
			vVec = *((TV3*)pVertices);
			//vVec.Transform( (TMatrix*)&pFrame->TransformationMatrix );
			m_BBox.AddPoint( &vVec );
			qRadiusAddPoint( &m_fBoundingRadius, &vVec );
			pVertices += uSizeOfFVF;
		}

		pMesh->UnlockVertexBuffer();
	}
	if( pFrame->pFrameSibling )
	{
		SubComputeBounds( pFrame->pFrameSibling );
	}

	if( pFrame->pFrameFirstChild )
	{
		SubComputeBounds( pFrame->pFrameFirstChild );
	}
}

/*
void SkinMeshD3D::ComputeBounds()
{
	m_fBoundingRadius = 0.0f;
	if( !m_pFrameRoot ) return;

	return SubComputeBounds( m_pFrameRoot );
}
*/
