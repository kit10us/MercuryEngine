// Copyright (c) 2003 - 2011 Quentin S. Smith
// All Rights Reserved

#include <dxi/SkinMesh.h>
#include <dxi/DirectXFile.h>

using namespace dxi;

SkinMeshRef::SkinMeshRef( SkinMesh * mesh )
: m_mesh( mesh )
{
	m_AnimInstance = *m_mesh->GetAnimationInstance( );
}

SkinMeshRef::~SkinMeshRef()
{
}

void SkinMeshRef::Render( RenderInfo & renderInfo )
{
	// Backup previous animation instance...
	AnimationInstance * pBackInstance = 0;
	pBackInstance = m_mesh->GetAnimationInstance();

	// Set our referenced meshes animation instance to ours
	m_mesh->SetAnimationInstance( &m_AnimInstance );
	
	// Render...
	m_mesh->Render( renderInfo );

	// Restore animation instance...
	m_mesh->SetAnimationInstance( pBackInstance );
}

void SkinMeshRef::Update( unify::Float fElapsed )
{
	m_AnimInstance.ElapseBy( fElapsed );
}

void SkinMeshRef::SetAnimationSet( unify::UInt32 uSet )
{
	m_AnimInstance.SetAnimationSet( uSet );
}

unify::UInt32 SkinMeshRef::GetAnimationSet() const
{
	return m_AnimInstance.m_uAnimationSet;
}

void SkinMeshRef::SetAnimationInstance( AnimationInstance* pInstance )
{
	m_AnimInstance = *pInstance;
}

AnimationInstance * SkinMeshRef::GetAnimationInstance()
{
	if( !m_AnimInstance.m_pAnimationSet ) m_AnimInstance.SetAnimationSet( m_AnimInstance.m_uAnimationSet );
	return &m_AnimInstance;
}

const unify::BBox< unify::Float > & SkinMeshRef::ComputeBounds()
{
	m_BBox = m_mesh->GetBBox();
	m_BSphere = m_mesh->GetBSphere();
	return m_BBox;
}

std::shared_ptr< SkinMesh > SkinMeshRef::GetMesh()
{
	return m_mesh;
}


////////////////////////////////////////////
// dxi::SkinMesh
////////////////////////////////////////////

dxi::SkinMesh::SkinMesh()
{
	m_AnimInstance.m_pAnimGroup = &m_AnimationGroup;
}

dxi::SkinMesh::SkinMesh( const std::string & filename )
: m_filename( filename )
{
	m_AnimInstance.m_pAnimGroup = &m_AnimationGroup;
}

dxi::SkinMesh::~SkinMesh()
{
	Destroy();
}

void dxi::SkinMesh::Create()
{
	CreateFromFilename( m_filename );
}

void dxi::SkinMesh::Destroy()
{
	if( Created() )
	{
		Invalidate();

		m_FrameHierarchy.Destroy();

		m_PrimitiveListOriginal.Destroy();
		m_PrimitiveListTemp.Destroy();
		m_NormalsVB.Release();
		m_AnimationGroup.Destroy();
	
		m_created = false;
	}
}

void dxi::SkinMesh::Validate()
{
	// TODO:
}

void dxi::SkinMesh::Invalidate()
{
}

// STEP 1: Load Frame heirarchy...
void dxi::SkinMesh::CreateFromFilename( const std::string & filenameIn )
{
	Destroy();

	std::string filename = filenameIn;
	if( filename.empty() )
	{
		throw qlib::Exception( "Not a valid filename to create skinned mesh! (\"" + filenameIn + "\")" );
	}

	DirectXFile file;

	file.LoadFile( filename );

	// data interaction variables...
	unify::DWord dwSize, dwOffset = 0;	// size of buffer (pData), current offset into buffer (reading) in bytes
	unify::Byte * pData = 0;

	unify::Matrix mMatrixTree[50];
	
	// Get frist object...
	file.GotoNextObject();
	
	MeshFrame * pFrame = 0;
	MeshFrame * pParentFrame = 0;

	AnimationSet * pCurrentAnimSet = 0;
	AnimationKey * pCurrentKey = 0;

	unify::Int32 iDepth = file.GetObjectLevel();

	while( ! file.EndOfFile() )
	{
		pFrame = 0;		
		
		/////////////////////////////////
		// Geometry Loading...
		/////////////////////////////////

		// Check if we have a frame (and exclude animation frame)...
		if( *file.GetGUID() == TID_D3DRMFrame )
		{
			pFrame = m_FrameHierarchy.AddMeshFrame( file.GetCurrentName() );

			// Get matrix...
			file.GotoChildObject();
			if( *file.GetGUID() == TID_D3DRMFrameTransformMatrix )
			{
				file.GetData( NULL, &dwSize, (void**)&pData );
				dwOffset = 0;
				unify::Matrix matrix;
				memcpy( &matrix, pData+dwOffset, sizeof( unify::Matrix ) );
				pFrame->SetModelMatrix( matrix );
				dwOffset += sizeof( unify::Matrix );
				//*pFrame->GetModelMatrix() = *((TMatrix*)pData);
			}
			else
			{
				//AssertMessage( "No transformation matrix found for frame!" );
			}
		} // if( *file.GetGUID() == TID_D3DRMFrame )


		
		/////////////////////////////
		// Animation Loading...
		/////////////////////////////

		// Check if we have a skin weight...
		else if( *file.GetGUID() == TID_D3DRMSkinWeights )
		{
			dwOffset = 0;
			file.GetData( 0, &dwSize, (void**)&pData );

			unify::Char** ppszFrameName;
			file.GetData( 0, &dwSize, (void**)&ppszFrameName );
			//ppszFrameName 
			dwOffset += 4;
			MeshFrame* pFrame = m_FrameHierarchy.FindFrame( *ppszFrameName );

			if( !pFrame )
			{
				// ERROR
			}

			unify::UInt32 uNumInfluences = GetDataAsType( unify::UInt32, pData, dwOffset );
			if( !pFrame->CreateInfluences( uNumInfluences ) )
			{
				// ERROR
			}

			unify::UInt32 u;
			for( u = 0; u < uNumInfluences; u++ )
			{
				unify::UInt32 uVert = GetDataAsType( unify::UInt32, pData, dwOffset );
				pFrame->SetVertexToInfluence( u, uVert );
			}

			for( u = 0; u < uNumInfluences; u++ )
			{
				unify::Float fInf = GetDataAsType( unify::Float, pData, dwOffset );
				pFrame->SetVertexInfluence( u, fInf );
			}

			memcpy( pFrame->GetMatrixOffset(), (pData+dwOffset), sizeof( unify::Matrix ) );
			dwOffset += sizeof( unify::Matrix );
		}

		// Check if we have an AnimationSet.
		// An AnimationSet triggers the creation of an animation set within the mesh's animation group.
		else if( *file.GetGUID() == TID_D3DRMAnimationSet )
		{
			pCurrentAnimSet = m_AnimationGroup.AddAnimationSet();
		} // else if( *file.GetGUID() == TID_D3DRMAnimationSet )

		// Check for an "Animation"...
		else if( *file.GetGUID() == TID_D3DRMAnimation )
		{
			// DO NOTHING
		} // else if( *file.GetGUID() == TID_D3DRMAnimation )

		// Chech for an animation key...
		if( *file.GetGUID() == TID_D3DRMAnimationKey )
		{
			dwOffset = 0;
			file.GetData( 0, &dwSize, (void**)&pData );
			unify::UInt32 uType = GetDataAsType( unify::UInt32, pData, dwOffset );
			unify::UInt32 uNumSteps = GetDataAsType( unify::UInt32, pData, dwOffset );
			
			pCurrentKey = pCurrentAnimSet->AddAnimationKey( uNumSteps, (AnimationKeyType)uType );

			unify::UInt32 uStep, uTime, uNumFloats;
			switch( uType )
			{
			case AnimationKeyRotation:
				break;
			case AnimationKeyScale:
			case AnimationKeyPosition:
				break;

			case AnimationKeyMatrix:
			case AnimationKeyMatrix2:
				for( uStep = 0; uStep < uNumSteps; uStep++ )
				{
					uTime = GetDataAsType( unify::UInt32, pData, dwOffset );
					uNumFloats = GetDataAsType( unify::UInt32, pData, dwOffset );
					unify::Matrix matrix = GetDataAsType( unify::Matrix, pData, dwOffset );
					pCurrentKey->SetKeyStep( uStep, (void*)&matrix );
					pCurrentAnimSet->SetKeyStepTime( uStep, uTime * 0.00025f );
				}
				break;
			}

			// Check for our relative frame...
			file.GotoNextObject();
			MeshFrame* pFrame = m_FrameHierarchy.FindFrame( file.GetCurrentName() );

			assert( pFrame && "Frame not found in heirarchy!" );

			pCurrentKey->LinkToFrame( pFrame );
		} // if( *file.GetGUID() == TID_D3DRMAnimationKey )
	

		// Move to next data object...
		if( !file.GotoNextObject() ) break;
		unify::TInt32 iNewDepth = file.GetObjectLevel();

		// Check for traversal of frame heirarchy...
		if( pFrame )
		{
			// Check if we are now deeper
			if( iNewDepth > iDepth )
			{
				// We are now at our first child (second to matrix)
				// DO NOTHING...
			}

			// Check if we are higher, or at the same level
			// so we need to backwards traverse to guarantee the next frame attaches to it's proper place.
			else if( iNewDepth <= iDepth )
			{
				unify::TInt32 i;
				for( i = 0; i < ((iDepth - iNewDepth) + 1); i++ )
				{
					m_FrameHierarchy.GotoParent();
				}
			}

			// Check if we are at the same depth
			else
			{
				// We are dealing with a sibling of the last frame, so make sure it will attach to our present parent...
				m_FrameHierarchy.GotoParent();
			}
			iDepth = iNewDepth;
		}// if( pFrame )

	} // file.GetCurrentData()

	file.Release();

	// Load primtive list...
	LoadMesh( filename, &m_PrimitiveListOriginal );

	// Duplicate information in the temp primitive list...
	m_PrimitiveListTemp = m_PrimitiveListOriginal;

	m_created = true;
}

void SkinMesh::Render( RenderInfo & renderInfo )
{
	QShapeArgs args;
	args.SetRadius( 3 );
	QMesh * pShape = new QMesh( Shape_Sphere, args );

	unify::Matrix matWorld;
	matWorld = *GetDisplay()->GetTransform( Transform::World );
	
	// Loop through frames and render geometry from there
	MeshFrame * pFrame = m_FrameHierarchy.GetRoot();	
	
	unify::Matrix mTemp;
	unify::Matrix matEnd;
	unify::V3< unify::Float > vec, vecOrig;
	unify::UInt32 x, uVertex;
	unify::Float fInfluence;

	unify::UInt32 * pInfBuf = 0;	// Buffer to store the number of times a vertex has been influenced...
	// if 0 in the end then the verex result should be set to a non-influenced value (copied over directly)
	// the first influence is total (1), the subsequent are added to the first
	pInfBuf = new unify::UInt32[ m_PrimitiveListOriginal.GetNumVertices() ];
	ZeroMemory( pInfBuf, m_PrimitiveListOriginal.GetNumVertices() * sizeof( unify::UInt32 ) );
	
	
	////////////////////////////////////////////////////
	m_FrameHierarchy.GetRoot()->Update();	//????????
	////////////////////////////////////////////////////

	// Adhere vertices...
	m_PrimitiveListTemp = m_PrimitiveListOriginal;

	// Transform vertices...
	QVertexLock lock;
	m_PrimitiveListTemp.LockVertices( lock );

	QVertexLock lockOrig;
	m_PrimitiveListOriginal.LockVertices( lockOrig );

	// Default all temp (animation result) vertices to origin...
	for( uVertex = 0; uVertex < lock.uCount; uVertex ++ )
	{
		lock.SetMember( uVertex, &unify::V3< unify::Float >(0,0,0), FVF_XYZ );
	}

	// Traverse all frames updating temp verices.
	while( pFrame )
	{
		//GetDisplay()->SetTransform( TransformWorld0, pWorld );
		//GetDisplay()->PrepareFixedFunctionTransform();

		//qMatrixMultiply( &matEnd,  pFrame->GetLocalMatrix(), &matWorld );
		//qMatrixMultiply( &matEnd,  pFrame->GetModelMatrix(), &matWorld );
		//GetDisplay()->SetTransformIm( TransformWorld0, &matEnd );
		//pShape->Render( NULL );

		// Build some matrices...
		unify::MatrixIdentity mFinal, mInverse;

		/*
		if( pFrame->GetParent() )
		{			
			qMatrixInverse( &mInverse, NULL, ((MeshFrame*)pFrame->GetParent())->GetLocalMatrix() );
		}
		qMatrixMultiply( &mFinal, pFrame->GetModelMatrix(), &mInverse );
		*/
		//qMatrixMultiply( &mFinal, pFrame->GetLocalMatrix(), pFrame->GetMatrixOffset() );
		//qMatrixMultiply( &mFinal, pFrame->GetMatrixOffset(), pFrame->GetLocalMatrix() );
		//qMatrixMultiply( &mFinal, pFrame->GetMatrixOffset(), pFrame->GetModelMatrix() );
		//qMatrixMultiply( &mFinal, pFrame->GetModelMatrix(), pFrame->GetMatrixOffset() );
		//qMatrixInverse( &mFinal, NULL, pFrame->GetMatrixOffset() );
		mFinal = unify::MatrixIdentity() * pFrame->GetMatrixOffset();

		for( x = 0; x < pFrame->GetNumInfluences(); x++ )
		{
			uVertex = pFrame->GetVertexToInfluence( x );
			fInfluence = pFrame->GetVertexInfluence( x );

			lockOrig.GetMember( uVertex, &vecOrig, FVF_XYZ );
			lock.GetMember( uVertex, &vec, FVF_XYZ );
			mFinal.TransformCoord( vecOrig );
			if( pInfBuf[uVertex] == 0 )
			{
				mFinal.TransformCoord( vecOrig );
				vec = vecOrig * fInfluence;
			}
			else
			{
				vec += vecOrig * fInfluence;
			}
			lock.SetMember( uVertex, &vec, FVF_XYZ );
			pInfBuf[ uVertex ] += 1;

			/*
			// Method 1...
			qMatrixMultiply( &mTemp, pFrame->GetModelMatrix(), &mInvOffset );
			qMatrixMultiply( &mTemp, pFrame->GetMatrixOffset(), pFrame->GetModelMatrix() );
			qMatrixMultiply( &mTemp, pFrame->GetModelMatrix(), pFrame->GetMatrixOffset() );
			*/
			
			/*
			// Method 2...
			qMatrixMultiply( &mTemp, pFrame->GetLocalMatrix(), pFrame->GetMatrixOffset() );
			lockOrig.GetMember( uVertex, &vecOrig, FVF_XYZ );
			lock.GetMember( uVertex, &vec, FVF_XYZ );

			vecOrig.Transform( &mTemp );
			vecOrig *= fInfluence;
			vec += vecOrig;

			lock.SetMember( uVertex, &vec, FVF_XYZ );
			*/
		}		

		// Move to next...
		if( pFrame->GetChild() )
		{
			pFrame = (MeshFrame*)pFrame->GetChild();
		}
		else if( pFrame->GetNext() )
		{
			pFrame = (MeshFrame*)pFrame->GetNext();
		}
		else
		{
			MeshFrame * pParent = (MeshFrame*)pFrame->GetParent();
			while( pParent && !pParent->GetNext() )
			{
				pParent = (MeshFrame*)pParent->GetParent();
			}
			if( pParent )
			{
				pFrame = (MeshFrame*)pParent->GetNext();
			}
			else
			{
				pFrame = 0;
			}
		}
	}

	SafeDelete( pInfBuf );
	SafeDelete( pShape );

	m_PrimitiveListTemp.UnlockVertices( true );
	m_PrimitiveListOriginal.UnlockVertices( false );

	GetDisplay()->SetTransform( Transform::World, &matWorld );
	m_PrimitiveListTemp.Render( renderInfo );
}

bool SkinMesh::SetTime( unify::Double dTime )
{
	m_AnimInstance.m_dTime = dTime;
	m_AnimInstance.m_pAnimationSet->SetTime( (unify::Float)dTime );
	return TRUE;
}

unify::Double SkinMesh::GetTime() const
{
	return m_AnimInstance.m_dTime;
}

AnimationSet* SkinMesh::SetAnimationSet( unify::UInt32 uAnimationSet, AnimationSet* pAnimationSet )
{
	m_AnimInstance.m_uAnimationSet = uAnimationSet;
	m_AnimInstance.m_pAnimationSet = m_AnimationGroup.SetAnimationSet( uAnimationSet, pAnimationSet );
	return m_AnimInstance.m_pAnimationSet;
}

// Sets the curent animation set and time.
// Encapsulates SetAnimationSet & SetTime.
void SkinMesh::SetAnimationInstance( AnimationInstance* pInstance )
{
	m_AnimInstance = *pInstance;
	m_AnimInstance.m_pAnimationSet->SetTime( (unify::Float)m_AnimInstance.m_dTime );
}

AnimationInstance * SkinMesh::GetAnimationInstance()
{
	return &m_AnimInstance;
}

void SkinMesh::Update( unify::Float fElapsed )
{
	m_AnimInstance.ElapseBy( fElapsed );
}

MeshFrameHierarchy * SkinMesh::GetMeshFrameHierarchy()
{
	return &m_FrameHierarchy;
}