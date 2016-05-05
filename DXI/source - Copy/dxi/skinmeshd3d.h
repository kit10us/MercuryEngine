// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi\geo\Geometry.h>
#include <dxi\VertexBuffer.h>


namespace dxi
{
	// enum for various skinning modes possible
	enum METHOD 
	{
		D3DNONINDEXED,
		D3DINDEXED,
		SOFTWARE,
		D3DINDEXEDVS,
		D3DINDEXEDHLSLVS,
		NONE
	};


	struct D3DXFRAME_DERIVED: public D3DXFRAME
	{
		unify::Matrix        CombinedTransformationMatrix;
	};

	struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified    
	                                
		// SkinMesh info             
		LPD3DXMESH			    pOrigMesh;
		LPD3DXATTRIBUTERANGE	pAttributeTable;
		unsigned long			NumAttributeGroups; 
		unsigned long			NumInfl;
		LPD3DXBUFFER		    pBoneCombinationBuf;
		unify::Matrix**			ppBoneMatrixPtrs;
		unify::Matrix*			pBoneOffsetMatrices;
		unsigned long			NumPaletteEntries;
		bool				    UseSoftwareVP;
		unsigned long			iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
	};

	class SkinMeshD3DRef;
	class SkinMeshD3D;

	/*
	 * References a skinned mesh allowing more than one of the same mesh to exist with different concurrent animation periods
	 */
	class SkinMeshD3DRef : public dxi::Geometry
	{
	public:
		SkinMeshD3DRef( SkinMeshD3D* pMesh );
		~SkinMeshD3DRef();

		void Render( RenderInfo* pInfo = 0 );
		void Update( float fElapsed );

		virtual const unify::BBox< float > & ComputeBounds();

		SkinMeshD3D* GetMesh();

	protected:
		SkinMeshD3D* m_pMesh;
		unsigned int m_uAnimationSet;
		double m_dTime;
	};


	class SkinMeshD3D : public Geometry
	{    
	public:
		SkinMeshD3D();
		SkinMeshD3D( const char* pszFilename );
		virtual ~SkinMeshD3D();

		bool LoadSkinnedMesh( const char* pszFilename );
		void Render( RenderInfo* pInfo = 0 );
		void Update( unify::Seconds elapsed );

		//const unify::BBox< float > & ComputeBounds();

		// CD3DApp mini-API
		HRESULT InitDeviceObjects(const char*            strMeshPath,
								  char*            strSkinnedMeshFXPath,
								  D3DXVECTOR3*      vObjectCenter,
								  float*            fObjectRadius);
		HRESULT RestoreDeviceObjects();
		HRESULT InvalidateDeviceObjects();
		HRESULT DeleteDeviceObjects();

		//public helpers
		METHOD       getSkinningMethod() { return m_SkinningMethod; };
		LPD3DXFRAME  getFrameRoot() { return m_pFrameRoot; };

		HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED *pMeshContainer,
									 METHOD                     SkinningMethod );
		
		void UpdateSkinningMethod( LPD3DXFRAME       pFrameBase,
								   METHOD            SkinningMethod );

		// Animation Time...
		bool SetTime( double dTime );
		double GetTime() const; 

		// Info retrival...
		const char* GetFilename() const;
		unsigned int GetNumBones() const;
		unsigned int GetNumBoneMatricesMax();
		unify::Matrix* GetBonesMatrices();
		LPD3DXFRAME GetFrameRoot();
		unsigned int GetNumAnimations();

	protected:
		LPD3DXFRAME                 m_pFrameRoot;
		LPD3DXANIMATIONCONTROLLER   m_pAnimController;    

		METHOD                      m_SkinningMethod;   // Current skinning method
		unify::Matrix*					m_pBoneMatrices;
		unsigned int                     m_uNumBoneMatricesMax;

		unify::Matrix						m_mWorld;

		char*	m_pszFilename;
		unsigned int m_uNumBones;

		//LPDIRECT3DVERTEXSHADER9     m_pIndexedVertexShader[4];
		LPD3DXEFFECT                m_pEffect;
		
		void DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, const unify::Matrix* pmatView );
		void DrawFrame( LPD3DXFRAME       pFrame, const unify::Matrix*        pmatView );

		HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainer );
		HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
		void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, const unify::Matrix* pParentMatrix = 0 );

		void SubComputeBounds( const D3DXFRAME* pFrame );
	};
}