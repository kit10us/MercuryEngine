// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <objbase.h>
#include <rmxfguid.h>
#include <dxfile.h>
#include <unify/Unify.h>
#include <unify/Matrix.h>
#include <unify/Path.h>

#pragma comment( lib, "D3dxof.lib" )

namespace dxi
{
	// TODO: Move implementation to a seperate file

	#define GetDataAsTypeAndIncrementOffset(_type,_data,_offset)					*((_type*)&_data[_offset]);	_offset += sizeof(_type)
	#define GetDataAsTypeAndIncrementOffsetArray(_type,_data,_offset,_count)		((_type*)&_data[_offset]);	_offset += sizeof(_type) * _count


	#define QDEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
			const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

	/*{ 3CF169CE-FF7C-44ab-93C0-F78F62D172E2 } */
	QDEFINE_GUID(TID_D3DRMXSkinMeshHeader,
	0x3cf169ce, 0xff7c, 0x44ab, 0x93, 0xc0, 0xf7, 0x8f, 0x62, 0xd1, 0x72, 0xe2);

	/* { 6f0d123b-bad2-4167-a0d0-80224f25fabb } */
	QDEFINE_GUID(TID_D3DRMSkinWeights,
	0x6f0d123b, 0xbad2, 0x4167, 0xa0, 0xd0, 0x80, 0x22, 0x4f, 0x25, 0xfa, 0xbb);

	/* { b8d65549-d7c9-4995-89cf-53a9a8b031e3 } */
	QDEFINE_GUID(TID_D3DRMVertexDuplicationIndices,
	0xb8d65549, 0xd7c9, 0x4995, 0x89, 0xcf, 0x53, 0xa9, 0xa8, 0xb0, 0x31, 0xe3);

	typedef enum DXFType
	{
		DXFNull,
		DXFReference,
		DXFData,
		DXFBinary
	} DXFType;

	class DirectXFile
	{
	public:
		struct SkinWeights {
			std::string frameName;
			std::vector< size_t > vertices;
			std::vector< float > weights;
			unify::Matrix offset;
		};

		DirectXFile();
		DirectXFile( const unify::Path & filePath );
		~DirectXFile();

		void Release();

		void LoadFile( const unify::Path & filePath );

		bool GotoNextObject();	
		bool GotoRootObject();
		bool GotoChildObject();
		bool GotoSiblingObject();
		bool GotoParentObject();

		const std::string & GetCurrentName() const;
		const GUID & GetGUID() const;
		DXFType GetObjectType() const;
		int GetObjectLevel() const;
		bool GetType( const GUID **ppguid ) const;
		bool GetData( const std::string & name, unsigned long & size, void ** data );

		bool EndOfFile();

		bool GetData( std::string & dataString );
		bool GetData( unify::Matrix & matrix );
		bool GetData( SkinWeights & skinWeights );
	
		static unsigned char* GetXTemplates();
		static unsigned int GetXTemplateBytes();
		static std::string GetGUIDName( const GUID* pGUID );

	protected:
		typedef enum SearchStatus
		{
			SearchRoot,
			SearchChild,
			SearchSibling,
			SearchParent,
			SearchEnd
		} SearchStatus;

		struct DataPack
		{
			DataPack()
			{ 
				pObject = 0;
				pData = 0;
				pBinary = 0;
				type = DXFNull;
			}
			DataPack( IDirectXFileObject* pNewObject, IDirectXFileData* pNewData, DXFType newtype )
			{ 
				pObject = pNewObject;
				pData = pNewData;
				pBinary = 0;
				type = newtype;
			}
			DataPack( IDirectXFileObject* pNewObject, IDirectXFileBinary* pNewBinary, DXFType newtype )
			{ 
				pObject = pNewObject;
				pData = 0;
				pBinary = pNewBinary;
				type = newtype;
			}

			void Release()
			{
				type = DXFNull;
			}

			const GUID** GetGUID( const GUID** ppguid ) const
			{
				if( pData )
				{
					pData->GetType( ppguid );
				}

				return ppguid;
			}
			
			std::string GetName() const
			{
				char szName[ 256 ];
				unsigned long dwLength = 256;
				if( pData ) 
					pData->GetName( szName, &dwLength );

				if( pBinary )
					pBinary->GetName( szName, &dwLength );

				return std::string( szName );
			}

			bool GetData( const std::string & name, unsigned long & size, void ** data )
			{
				if( ! pData )
				{
					return false;
				}
		
				HRESULT hr = pData->GetData( name.empty() ? 0 : name.c_str(), &size, data );
				return SUCCEEDED( hr );
			}
			
			bool Build( IDirectXFileObject* pNewObject )
			{
				IDirectXFileDataReference* pDataRef = 0;
				
				pObject = pNewObject;
				pData = 0;
				pBinary = 0;
				type = DXFNull;

				if( ! pNewObject ) return false;

				// Reference...
				if( SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileDataReference, (void**)&pDataRef )) )
				{
					if( FAILED(pDataRef->Resolve( &pData )) )
					{
						if( pDataRef )
						{
							pDataRef->Release();
							pDataRef = 0;
						}
						return false;
					}
					type = DXFReference;
					return true;
				}
				
				// Data...
				if( SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileData, (void**)&pData )) )
				{
					type = DXFData;
					return true;
				}

				// Data Binary...
				if( SUCCEEDED(pObject->QueryInterface( IID_IDirectXFileBinary, (void**)&pBinary ) ) )
				{
					type = DXFBinary;
					return true;
				}

				return FALSE; // Error - Not a valid interface!
			}

			bool Build( IDirectXFileData* pNewData )
			{
				pObject = (IDirectXFileObject*)pNewData;
				pData = pNewData;
				pBinary = 0;
				type = DXFNull;
				return pNewData ? true : false ;
			}

			IDirectXFileObject** GetNextObject( IDirectXFileObject** ppObject )
			{
				if( pData )
				{
					pData->GetNextObject( ppObject );
				}
				return ppObject;
			}

			IDirectXFileObject* pObject;
			IDirectXFileData* pData;
			IDirectXFileBinary* pBinary;
			DXFType type;
		};

		// Increment our level, set our backup parent, our parent and the new current.
		bool IncrementLevel( IDirectXFileObject* pNewObject );

		// Move back to a previous level, setting parent, decrementing level, setting current.
		bool DecrementLevel();
		
		IDirectXFile*			m_pDXFile;
		IDirectXFileEnumObject*	m_pDXEnum;
		unify::Path m_filePath;
		
		DataPack m_ParentPackBackup[50];	// top level parent backup == parent
		DataPack m_CurrentPack;
		DataPack m_ParentPack;

		SearchStatus	m_SearchStatus;	
		unsigned int			m_iLevel;
		mutable const GUID * m_GUID;
		mutable std::string	m_sName;

		static unsigned int m_D3DRM_XTEMPLATE_BYTES;
		static unsigned char m_D3DRM_XTEMPLATES[];
	};
}