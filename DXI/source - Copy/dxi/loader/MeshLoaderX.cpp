// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/loader/MeshLoaderX.h>
#include <unify/String.h>
#include <stack>

using namespace dxi;
using namespace loader;

void MeshLoaderX::LoadMesh( const unify::Path & filePath, PrimitiveList & primitiveList, Effect::shared_ptr effect, Managers managers )
{
    // Clear any existing mesh data, as we are starting from scratch...
    // Q: Perhaps we can append in the future? This might require some PrimitiveList arithmitic.
	primitiveList.Destroy();

    // Pass 1: 
    LoadMesh_Pass1( filePath, primitiveList, effect, managers );
}

void MeshLoaderX::LoadMesh_Pass1( const unify::Path & filePath, PrimitiveList & primitiveList, Effect::shared_ptr effect, Managers managers )
{
	DirectXFile file( filePath );

	std::vector< std::shared_ptr< TempMeshType > > meshList;
	TempMeshType * currentMesh = 0;

	unsigned int numMaterials = 0;

	unsigned long dwSize, dwOffset = 0;	// size of buffer (pData), current offset into buffer (reading) in bytes
	unsigned char * pData = 0;

	unify::FrameSet & frameSet = primitiveList.GetFrameSet();
	std::stack< size_t > parentIndex;
	std::string frameName;

	dxi::frameanimation::AnimationSet & animationSet = primitiveList.GetAnimationSet();
	
	int currentNodeDepth = -1;
	file.GotoNextObject();
	while( !file.EndOfFile() )
	{
		if( file.GetGUID() == TID_D3DRMFrame )
		{
			size_t currentLevel = file.GetObjectLevel();
			if( ! parentIndex.empty() && currentLevel )
			{
				while( parentIndex.size() > currentLevel )
				{
					parentIndex.pop();
				}
			}

			std::string name = file.GetCurrentName();
			frameName = name;
			file.GotoNextObject();
		}

		// Maintain the matrix tree...
		else if( file.GetGUID() == TID_D3DRMFrameTransformMatrix )
		{
			unify::Matrix matrix;
			file.GetData( matrix );
			size_t index = frameSet.Add( matrix, parentIndex.empty() ? 0 : parentIndex.top(), frameName );
			parentIndex.push( index ); 
			file.GotoNextObject();
		}

		// Search for ALL meshes
		else if( file.GetGUID() == TID_D3DRMMesh )
		{
			int iMeshLevel = file.GetObjectLevel()+1;

			// Add a mesh.
			currentMesh = new TempMeshType;
			meshList.push_back( std::shared_ptr< TempMeshType >( currentMesh ) );

			// Compile matrices into one...
			//currentMesh->mEnd = parentIndex.empty() ? unify::Matrix::MatrixIdentity() : frameSet.Local( parentIndex.top() );

			// Load vertices...
			{
				dwOffset = 0;
				file.GetData( "", dwSize, (void**)&pData );
				size_t numberOfVertices = GetDataAsTypeAndIncrementOffset( unsigned int, pData, dwOffset );
				currentMesh->vertexDetails.resize( numberOfVertices );

				for( size_t i = 0; i < numberOfVertices; ++i )
				{
					currentMesh->vertexDetails[ i ].vertex.pos = GetDataAsTypeAndIncrementOffset( unify::V3< float >, pData, dwOffset );
				}
			}

			// Load faces...
			{
				unsigned int numberOfFaces = GetDataAsTypeAndIncrementOffset( unsigned int, pData, dwOffset );
				currentMesh->faces.resize( numberOfFaces );

				Index32 index = 0;
				for( size_t face = 0; face != numberOfFaces; ++face )
				{
					// Note that we are simply duplicating data in the mesh (might want to do this in a specific geometric way.
					unsigned int faceSize = GetDataAsTypeAndIncrementOffset( unsigned int, pData, dwOffset );
					currentMesh->faces[ face ].resize( faceSize );

					unsigned int * pFace = GetDataAsTypeAndIncrementOffsetArray( unsigned int, pData, dwOffset, faceSize );
					for( size_t i = 0; i != faceSize; ++i )
					{
						currentMesh->faces[ face ][ i ] = static_cast< Index16 >( pFace[ i ] );
					}
				}
			}

			// Handle extra mesh elements (TextureCoords, Materials, Normals)...
			file.GotoNextObject();
			while( !file.EndOfFile() && file.GetObjectLevel() == iMeshLevel )
			{
				if( file.GetGUID() == TID_D3DRMMeshNormals )
				{
					file.GetData( "", dwSize, (void**)&pData );
					dwOffset = 0;

					size_t numberOfVertices = GetDataAsTypeAndIncrementOffset( unsigned int, pData, dwOffset );
					assert( currentMesh->vertexDetails.size() == numberOfVertices );
					for( size_t i = 0; i < numberOfVertices; ++i )
					{
						currentMesh->vertexDetails[ i ].vertex.normal = GetDataAsTypeAndIncrementOffset( unify::V3< float >, pData, dwOffset );
					}
				}

				else if( file.GetGUID() == TID_D3DRMMeshTextureCoords )
				{
					file.GetData( "", dwSize, (void**)&pData );
					dwOffset = 0;
					size_t numberOfVertices = GetDataAsTypeAndIncrementOffset( unsigned int, pData, dwOffset );

					for( size_t i = 0; i < numberOfVertices; ++i )
					{
						currentMesh->vertexDetails[ i ].vertex.coords = GetDataAsTypeAndIncrementOffset( unify::TexCoords, pData, dwOffset );
					}
				}

				else if( file.GetGUID() == TID_D3DRMVertexDuplicationIndices )
				{
					// Not handled
				}

				else if( file.GetGUID() == TID_D3DRMSkinWeights )
				{
					DirectXFile::SkinWeights skinWeights;
					file.GetData( skinWeights );

					assert( skinWeights.vertices.size() == skinWeights.weights.size() );

					size_t frameIndex = frameSet.Find( skinWeights.frameName );
					for( size_t i = 0, end = skinWeights.vertices.size(); i != end; ++i )
					{
						size_t vertexIndex = skinWeights.vertices[ i ];
						float weight = skinWeights.weights[ i ];

						// If we don't have one, add one.
						currentMesh->vertexDetails[ vertexIndex ].AddFrameAndWeight( FrameAndWeight( frameIndex, weight ) );
					}
				}

				else if( file.GetGUID() == TID_D3DRMMeshMaterialList )
				{
					// Not supported.
				}
				file.GotoNextObject();
			}
		}

		else if( file.GetGUID() == TID_D3DRMAnimationSet )
		{
			std::string animationName = file.GetCurrentName();

			int parentLevel = file.GetObjectLevel();
			file.GotoChildObject();
			while( ! file.EndOfFile() && file.GetObjectLevel() == parentLevel + 1 )
			{
				if( file.GetGUID() == TID_D3DRMAnimation )
				{
					int x(0);x;
				}
				file.GetData( "", dwSize, (void**)&pData );
				file.GotoNextObject();
			}
		}
		else
		{
			file.GotoNextObject();
		}
	}

	file.Release();

	// We've search for all meshes and mesh data, now create primitive list...
	assert( meshList.size() );

	size_t totalNumberOfVertices = 0;
	size_t totalNumberOfIndices = 0;

    // For tracking the root/base vertex for each mesh.
	std::vector< size_t > baseVertex( meshList.size() );

    // Calculate complete expected buffer sizes.
	for( size_t m = 0, mend = meshList.size(); m != mend; ++m )
	{
		TempMeshType & mesh = *meshList[ m ].get();

        // Count number of vertices...
		baseVertex[ m ] = totalNumberOfVertices;
		totalNumberOfVertices += mesh.vertexDetails.size();

		// Count faces...
		for( size_t f = 0, fend = mesh.faces.size(); f != fend; ++f )
		{
            // Since we only draw triangles, and triangles have three points, we will add (size - 2) triangles, and traingle * 3 indices.
            assert( mesh.faces[ f ].size() >= 3 );
            size_t trianglesForThisFace = mesh.faces[ f ].size() - 2;

			totalNumberOfIndices += trianglesForThisFace * 3;
		}
	}


	// Create vertex and index buffers...
	qjson::Object jsonFormat;
	jsonFormat.Add( { "Position", "Float3" } );
	jsonFormat.Add( { "Normal", "Float3" } );
	jsonFormat.Add( { "Diffuse", "Color" } );
	jsonFormat.Add( { "Specular", "Color" } );
	jsonFormat.Add( { "TexCoord", "TexCoord" } );
	dxi::VertexDeclaration vd( jsonFormat );
	VertexBuffer & vb = primitiveList.GetBufferSet( 0 ).GetVertexBuffer();
	vb.Create( totalNumberOfVertices, vd, BufferUsage::Default );

    // Create index buffer...
    IndexBuffer & ib = primitiveList.GetBufferSet( 0 ).GetIndexBuffer();
	ib.Create( totalNumberOfIndices, IndexFormat::Index16, BufferUsage::Default );

	{
		unify::DataLock vertexLock;
		vb.Lock( vertexLock );
		IndexLock indexLock;
		ib.Lock( indexLock );

		size_t index = 0;

		for( size_t m = 0, mend = meshList.size(); m != mend; ++m )
		{
			TempMeshType & mesh = *meshList[ m ].get();

			for( size_t v = 0, vend = mesh.vertexDetails.size(); v != vend; ++v )
			{
				vd.WriteVertex( vertexLock,  v + baseVertex[ m ], vd, (void*)&mesh.vertexDetails[ v ].vertex );
			}

			for( size_t f = 0, fend = mesh.faces.size(); f != fend; ++f )
			{
				const std::vector< Index16 > & face = mesh.faces[ f ];
				size_t trianglesForThisFace = face.size() - 2;

				for( size_t t = 0, tend = trianglesForThisFace; t != tend; ++t )
				{
					// This configuration assumes a clockwise order, every triangle uses the first point as a starting point.
					indexLock.SetIndex( index++, face.at( 0 ) );
					indexLock.SetIndex( index++, face.at( t + 1 ) );
					indexLock.SetIndex( index++, face.at( t + 2 ) );
				}
			}
		}
		ib.Unlock();
		vb.Unlock();
		assert( index == totalNumberOfIndices );
	}

	// TODO:

	/*
	Effect & effect = primitiveList.AddEffect( "effect1" );

	// Setup Effect material...
	effect.GetMaterial() = meshList[ 0 ]->materialList.materials[ 0 ];
	effect.SetFVF( Vertex::FORMAT );

	// Build method...
	unsigned int primitiveCount = totalNumberOfIndices / 3;
	primitiveList.AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 0, totalNumberOfVertices, 0, primitiveCount, 0, true ) );
	*/

	/*
	unsigned int uNumVertices = 0;
	unsigned int uNumFaces = 0;
	unsigned int numberOfIndices = 0;
	unsigned int uMesh;
	pCurrentMesh = pFirstMesh;
	for( uMesh = 0; uMesh < uNumMeshes; uMesh++ )
	{
		pCurrentMesh->uVerticeOffset = uNumVertices;
		uNumVertices += pCurrentMesh->uNumVertices;
		uNumFaces += pCurrentMesh->uNumFaces;
		numberOfIndices += pCurrentMesh->uNumFaces * (3 * (pCurrentMesh->faceSize - 2));

		// Transform vertices...
		unsigned int u;
		for( u = 0; u < pCurrentMesh->uNumVertices; u++ )
		{
			pCurrentMesh->mEnd.TransformCoord( pCurrentMesh->pVertexBuffer[u].pos );
			pCurrentMesh->mEnd.TransformNormal( pCurrentMesh->pVertexBuffer[u].normal );
		}
		pCurrentMesh = pCurrentMesh->pNextMesh;
	}

	// Create and Upload all vertices...
	VertexBuffer & vb = primitiveList.GetVertexBuffer();
	vb.Create( uNumVertices, Vertex::FORMAT, BufferUsage::Default );
	pCurrentMesh = pFirstMesh;
	unify::DataLock lock;
	vb.Lock( lock );
	for( uMesh = 0; uMesh < uNumMeshes; uMesh++ )
	{
		unsigned int u;
		for( u = 0; u < pCurrentMesh->uNumVertices; u++ )
		{
			lock.SetVertex( pCurrentMesh->uVerticeOffset + u, (unsigned char*)&pCurrentMesh->pVertexBuffer[u], Vertex::FORMAT );
		}
		pCurrentMesh = pCurrentMesh->pNextMesh;
	}

	vb.Unlock();

	// Count materials actually used...
	pCurrentMesh = pFirstMesh;
	unsigned int uNumUsedMaterials = 0;
	for( uMesh = 0; uMesh < uNumMeshes; uMesh++ )
	{
		for( size_t materialIndex = 0; materialIndex != pCurrentMesh->materialList.materials.size(); ++materialIndex )
		{
			if( ! pCurrentMesh->materialList.materialUseCount[ materialIndex ] )
			{
				continue;
			}
			uNumUsedMaterials++;
		}
		pCurrentMesh = pCurrentMesh->pNextMesh;
	}

	// Create all the materials as effects, create methods for each material/effect...
	IndexBuffer & ib = primitiveList.GetIndexBuffer();
	ib.Create( numberOfIndices, IndexFormat::Index16 );

	pCurrentMesh = pFirstMesh;
	unsigned int uMaterial = 0;
	unsigned int uStarIndex = 0;
	unsigned int uIndex = 0;
	IndexLock indexLock;
	ib.Lock( indexLock );
	Index16* pIndex = (Index16 *)indexLock.GetData();
	for( uMesh = 0; uMesh < uNumMeshes; uMesh++ )
	{
		// Add material...
		for( size_t i = 0; i < pCurrentMesh->materialList.materials.size(); ++i )
		{
			Effect & effect = primitiveList.AddEffect( "effect" + unify::StringFromInt( uMaterial ) );
			if( ! pCurrentMesh->materialList.materialUseCount[ i ] )
			{
				uMaterial++;	// Keep material count.
				continue;
			}

			// Setup Effect material...
			primitiveList.GetEffect( uMaterial ).GetMaterial() = pCurrentMesh->materialList.materials[ i ];
			primitiveList.GetEffect( uMaterial ).SetFVF( Vertex::FORMAT );

			// Build method...
			unsigned int primitiveCount = pCurrentMesh->materialList.materialUseCount[ i ] * (pCurrentMesh->faceSize - 2);
			primitiveList.AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, 0, uNumVertices, uIndex, primitiveCount, uMaterial, true ) );

			// Build Indices...
			for( unsigned int uFace = 0; uFace < pCurrentMesh->uNumFaces; uFace++ )
			{
				// If the face belongs to the present material, add it to the index buffer (sort by effect)...
				if( pCurrentMesh->materialList.faceMaterial[ uFace ] == (uMaterial - pCurrentMesh->firstMaterial) )
				{
					for( unsigned int u = 0; u < pCurrentMesh->faceSize - 2; ++u )
					{
						pIndex[ uIndex++ ] = (Index16)pCurrentMesh->uVerticeOffset + pCurrentMesh->pFaceBuffer[ uFace * pCurrentMesh->faceSize + 0 + u ];
						pIndex[ uIndex++ ] = (Index16)pCurrentMesh->uVerticeOffset + pCurrentMesh->pFaceBuffer[ uFace * pCurrentMesh->faceSize + 1 + u ];
						pIndex[ uIndex++ ] = (Index16)pCurrentMesh->uVerticeOffset + pCurrentMesh->pFaceBuffer[ uFace * pCurrentMesh->faceSize + 2 + u ];
					}
				}
			}
			uStarIndex = pCurrentMesh->materialList.materialUseCount[ i ] * 3;
			uMaterial++;
		}
		pCurrentMesh = pCurrentMesh->pNextMesh;
	}

	ib.Unlock();

	assert( uIndex == numberOfIndices );

/*
	bool debugOutputFrames =
#if 1
	true;
#else
	false;
#endif

	if( debugOutputFrames )
	{
		class DebugOutputFunctor : public unify::FrameSet::ForEachFunctor
		{
			const unify::FrameSet & m_frameSet;
		public:
			DebugOutputFunctor( const unify::FrameSet & frameSet )
				: m_frameSet( frameSet ) { }

			void operator()( size_t depth, size_t index )
			{
				std::string output;
				for( size_t i = depth; i ; --i )
				{
					output +=  ".";
				}
				output += unify::Cast< std::string >( index );
				output += ": \"";
				output += m_frameSet.Name( index );
				output += "\"";
				output += "\n";
				OutputDebugStringA( output.c_str() );
			}
		} functor( frameSet );

		frameSet.ForEach( functor );
	}
	*/
}
