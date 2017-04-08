// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Stream.h>

namespace unify 
{
	class FileStream : public Stream
	{
	public:
		FileStream();
		~FileStream();

		// TODO: Screw the void * here, this was to allow us to appear more robust, change this to a string.
		bool Open( StreamAccessType access, void * pData );
		void Close();
		
		unsigned int Read( void * pDest, unsigned int uLength );
		unsigned int ReadPack( void * pDest, unsigned int uLength );
		
		bool Write( const void * pSrc, unsigned int uLength );
		bool WritePack( const void * pSrc );

		bool Seek( StreamSeekType seek, int iOffset = 0 );

		__int64 Pos();
		bool SetPosition( __int64 );
	};

	__int64 FileLength( const char * pszFilename );
}
