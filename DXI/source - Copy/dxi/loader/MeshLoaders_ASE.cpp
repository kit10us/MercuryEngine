// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/MeshLoaders.h>
#include <dxi/loader/EffectLoader.h>
#include <dxi/ASEDocument.h>
#include <dxi/loader/MeshLoaderX.h>
#include <dxi/XMLConvert.h>
#include <unify/String.h>
#include <unify/FrameSet.h>

using namespace dxi;

void loader::LoadMesh_ASE( const unify::Path & filePath, PrimitiveList & primitiveList, dae::util::IEffectSolver * effectSolver, Managers managers )
{
	/*
	// Destroy existing geometry...
	primitiveList.Destroy();

	// Build vertex format, used by all vertices.
	//VertexFormat vfFormat = FVF::XYZ | FVF::Tex1 | FVF::Diffuse | FVF::Normal;
	VertexFormat vfFormat = FVF::XYZ | FVF::Tex1 | FVF::Normal;

	dxi::ASEDocument doc;
	Texture::shared_ptr texture;
	unsigned int u = 0;
	
	doc.Load( qlib::file::ToString( filePath ) );

	//////////////////////////////
	// Create materials...
	//////////////////////////////

	qxml::ElementList listMaterial;
	doc.FindElementsByTagNameRecursive( listMaterial, "MATERIAL" );
	std::vector< Effect::shared_ptr > effectTable;

	if( listMaterial.Count() == 0 )
	{
		// Create a default material...
		//primitiveList.AddEffect( "default" );
	}
	else
	{
		if ( ! effectManager )
		{
			effectManager.reset( new EffectManager );
		}

		for( u = 0; u < listMaterial.Count(); u++ )
		{
			qxml::Element * pElement;
			qxml::Element * pMaterial = listMaterial.Item( u );
			
			std::string name = pMaterial->GetElement( "MATERIAL_NAME" )->GetText();

			if ( ! effectManager->Exists( name ) )
			{
				Effect::shared_ptr effect = effectManager->Add( name, new Effect );
				effectTable.push_back( effect );

				effect->SetFVF( vfFormat );

				// Set basics (ambient, diffuse, specular, etc.)
				//pElement = pMaterial->GetElement( "MATERIAL_AMBIENT" );
				//unify::ColorUnit ambient = unify::ColorUnit::ColorUnitARGB( 1, pElement->GetAttribute( "R" )->GetFloat(), pElement->GetAttribute( "G" )->GetFloat(), pElement->GetAttribute( "B" )->GetFloat() );
				//pElement = pMaterial->GetElement( "MATERIAL_DIFFUSE" );
				//unify::ColorUnit diffuse = unify::ColorUnit::ColorUnitARGB( 1, pElement->GetAttribute( "R" )->GetFloat(), pElement->GetAttribute( "G" )->GetFloat(), pElement->GetAttribute( "B" )->GetFloat() );
				//pElement = pMaterial->GetElement( "MATERIAL_SPECULAR" );
				//unify::ColorUnit specular = unify::ColorUnit::ColorUnitARGB( 1, pElement->GetAttribute( "R" )->GetFloat(), pElement->GetAttribute( "G" )->GetFloat(), pElement->GetAttribute( "B" )->GetFloat() );
	
				// Texture...
				qxml::Element * pMap = pMaterial->GetElement( "MAP_DIFFUSE" );
				std::string sMapName = pMap->GetElement( "MAP_NAME" )->GetText();
				if( pMap != 0 )
				{
					pElement = pMap->GetElement( "BITMAP" );

					std::string textureName = pElement->GetText();

					Texture::shared_ptr texture;
					if ( textureManager && textureManager->Exists( textureName ) )
					{
						texture = textureManager->Find( textureName );
					}
					else
					{
						texture = textureManager->Add( textureName, new Texture( qlib::file::ChangeFilename( filePath, pElement->GetText() ) ) );
					}
					effect->SetTexture( 0, texture );
				}
			}
		}
	}

	//////////////////////////////////
	// Load Mesh Geometry...
	//////////////////////////////////

	// Grab the first object (TODO: Work on accessing all objects)
	qxml::Element * pGeomObject = doc.FindElement( "GEOMOBJECT" );
	qxml::Element * pMesh = doc.FindElement( "MESH" );

	unsigned int uNumFaces = unify::Cast< int >( pMesh->GetElement( "MESH_NUMFACES" )->GetText() );

	unsigned int uNumPVertices; // Positional "MESH_VERTEX"
	unsigned int uNumTVertices; // Texture "MESH_TVERT"

	uNumPVertices = (unsigned int)unify::Cast< int >( pMesh->GetElement( "MESH_NUMVERTEX" )->GetText() );
	uNumTVertices = (unsigned int)unify::Cast< int >( pMesh->GetElement( "MESH_NUMTVERTEX" )->GetText() );

	qxml::ElementList listFace;
	pMesh->FindElementsByTagNameRecursive( listFace, "MESH_FACE" );

	qxml::ElementList listTFace;
	pMesh->FindElementsByTagNameRecursive( listTFace, "MESH_TFACE" );

	// Position vertex index, Texture coord index, actual index (inique)...
	struct PT {
		int iP;
		int iT;

		PT( int p, int t )
		{
			iP = p;
			iT = t;
		}
	};

	// Load position and texture indices...
	std::vector< PT > listPT;
	listPT.reserve( uNumFaces * 3 );
	int posIndex[ 3 ];
	int texIndex[ 3 ];
	for( u = 0; u < uNumFaces; u++ )
	{
		// Positional face indices...
		posIndex[ 0 ] = listFace.Item( u )->GetAttribute( "A" )->GetInteger();
		posIndex[ 1 ] = listFace.Item( u )->GetAttribute( "B" )->GetInteger();
		posIndex[ 2 ] = listFace.Item( u )->GetAttribute( "C" )->GetInteger();

		// Texture face indices...
		texIndex[ 0 ] = listTFace.Item( u )->GetAttribute( "A" )->GetInteger();
		texIndex[ 1 ] = listTFace.Item( u )->GetAttribute( "B" )->GetInteger();
		texIndex[ 2 ] = listTFace.Item( u )->GetAttribute( "C" )->GetInteger();

		// Try the PT method...
		listPT.push_back( PT( posIndex[ 0 ], texIndex[ 0 ] ) );
		listPT.push_back( PT( posIndex[ 1 ], texIndex[ 1 ] ) );
		listPT.push_back( PT( posIndex[ 2 ], texIndex[ 2 ] ) );
	}
	
	// Create the vertex buffer for position and normals...
	unify::V3< float > * pVertex = new unify::V3< float >[ uNumPVertices ];

	qxml::ElementList listPosition;
	pMesh->FindElementsByTagNameRecursive( listPosition, "MESH_VERTEX" );

	// Grab positional and normal information...
	for( u = 0; u < listPosition.Count(); u++ )
	{
		int iPosIndex = listPosition.Item( u )->GetAttribute( "index" )->GetInteger();
		pVertex[ iPosIndex ].x = listPosition.Item( u )->GetAttribute( "x" )->GetFloat();
		pVertex[ iPosIndex ].y = listPosition.Item( u )->GetAttribute( "y" )->GetFloat();
		pVertex[ iPosIndex ].z = listPosition.Item( u )->GetAttribute( "z" )->GetFloat();
		bool bDebugBreak = true;
	}


	// Put the coords directly using thier indices and match pos and normals later
	qxml::ElementList listCoords; // Texture coordinates...
	pMesh->FindElementsByTagNameRecursive( listCoords, "MESH_TVERT" );
	unify::TexCoords * pCoords = new unify::TexCoords[ listCoords.Count() ];
	for( u = 0; u < listCoords.Count(); u++ )
	{
		pCoords[ u ].u = listCoords.Item( u )->GetAttribute( "u" )->GetFloat();
		pCoords[ u ].v = 1.0f - listCoords.Item( u )->GetAttribute( "v" )->GetFloat();
	}

	// Create vertex and index buffer...
	VertexBuffer & vb = primitiveList.GetVertexBuffer();
	vb.Create( (unsigned int)listPT.size(), vfFormat, BufferUsage::Default );
	unify::DataLock lock;
	vb.Lock( lock );

	IndexBuffer & ib = primitiveList.GetIndexBuffer();
	ib.Create( (unsigned int)listPT.size() * 3, IndexFormat::Index16, BufferUsage::Default );
	IndexLock indexLock;
	ib.Lock( indexLock );
	Index16* pIndex = (Index16 *)indexLock.GetData();

	qxml::ElementList listNormals;
	pMesh->FindElementsByTagNameRecursive( listNormals, "MESH_VERTEXNORMAL" );

	for( u = 0; u < listPT.size(); u++ )
	{
		int iP = listPT[ u ].iP;
		int iT = listPT[ u ].iT;

		unify::V3< float > vNormal;
		vNormal.x = listNormals.Item( u )->GetAttribute( "x" )->GetFloat();
		vNormal.y = listNormals.Item( u )->GetAttribute( "y" )->GetFloat();
		vNormal.z = listNormals.Item( u )->GetAttribute( "z" )->GetFloat();

		// Set vertex...
		lock.SetItemMember( u, &pVertex[ iP ], FVF::XYZ );
		lock.SetItemMember( u, &pCoords[ iT ], FVF::Tex1 );
		lock.SetItemMember( u, &vNormal, FVF::Normal );

		// Set index...
		pIndex[ u * 3 + 0 ] = u * 3 + 0;
		pIndex[ u * 3 + 1 ] = u * 3 + 1;
		pIndex[ u * 3 + 2 ] = u * 3 + 2;
	}

	vb.Unlock();
	ib.Unlock();
	delete [] pVertex;

	// Create methods...
	// Grab the material index (directly indexes the effect list)...
	qxml::Element * pMatRef = pGeomObject->GetElement( "MATERIAL_REF" );
	unsigned int effectIndex = 0;
	if( pMatRef )
	{
		effectIndex = unify::Cast< int >( pMatRef->GetText() );
	}

	primitiveList.AddMethod( RenderMethod( PrimitiveType::TriangleList, 0, listFace.Count() * 3, listFace.Count(), effectTable[ effectIndex ], true ) );
	*/
}
