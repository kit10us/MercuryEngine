// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/ResourceDesc.h>
#include <unify/Path.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace dxi;

ResourceDesc::ResourceDesc()
{
	m_bExtSearch = true;
}

ResourceDesc::ResourceDesc( const ResourceDesc & resourceDesc )
: m_sPathList( resourceDesc.m_sPathList )
, m_pathList( resourceDesc.m_pathList )
, m_sExtList( resourceDesc.m_sExtList )
, m_extensionList( resourceDesc.m_extensionList )
, m_bExtSearch( resourceDesc.m_bExtSearch )
{
}

ResourceDesc::ResourceDesc( const std::string & paths, const std::string & extensions )
{
	SetPaths( paths );
	SetExtensions( extensions );
	m_bExtSearch = true;
}

ResourceDesc::~ResourceDesc()
{
}

void ResourceDesc::AddPaths( const std::string & paths )
{
	if( paths.empty() ) return;

	// If we are not empty, ensure we will have a seperating semicolon if not prefixed to the input parameter.
	if( ! m_sPathList.empty() && paths[ 0 ] != ';' ) m_sPathList += ";";
	
	m_sPathList += paths;

	// Extract individual parts into our acutal list...
	if( ! m_sPathList.empty() )
	{
		unsigned int total = unify::ListPartCount( m_sPathList, ';' );
		for( unsigned int current = 0; current < total; ++current )
		{
			// Change extension...
			std::string path = unify::ListPart( m_sPathList, ';', current );
			m_pathList.push_back( path );
		}
	}
}

void ResourceDesc::AddExtensions( const std::string & extensions )
{
	if( extensions.empty() ) 
	{
		return;
	}

	// If we are not empty, ensure we will have a seperating semicolon if not prefixed to the input parameter.
	if( ! m_sExtList.empty() && extensions[ 0 ] != ';' ) m_sExtList += ";";
	
	m_sExtList += extensions;

	// Extract individual parts into our acutal list...
	if( ! m_sExtList.empty() )
	{
		unsigned int total = unify::ListPartCount( m_sExtList, ';' );
		for( unsigned int current = 0; current < total; ++current )
		{
			std::string extension = unify::ListPart( m_sExtList, ';', current );
			if( extension[ 0 ] != '.' )
			{
				extension = "." + extension;
			}
			m_extensionList.push_back( extension  );
		}
	}
}

// Checks if this resource description is linked to a given extension, caselessly.
bool ResourceDesc::HasExtension( const std::string & extension )
{
	for( unsigned int current = 0; current < NumExtensions(); ++current )
	{
		if( _stricmp( GetExtension( current ).c_str(), extension.c_str() ) == 0 ) return true;
	}
	return false;
}

// Search order... As Is, Existing Path {Extension List}, PathList {Extension List}
unify::Path ResourceDesc::CreateFilepath( const unify::Path & filePath )
{
	if( filePath.Empty() ) 
	{
		return 0;
	}

	unify::Path newFilePath = filePath;

	unsigned int uExt;
	unsigned int uExtCount = unify::ListPartCount( m_sExtList, ';' );

	unsigned int uPath = 0;
	unsigned int uPathCount = unify::ListPartCount( m_sPathList, ';' );

	// We accept our existing filename if the file exists (allowing forced texture selection)
	if( ! newFilePath.Exists() )
	{
		while( 1 )
		{
			// First check as is (note that this might generate a duplicate check if the extension is listed, should be
			// and inconsequential hit).
			if( newFilePath.Exists() )
			{
				return newFilePath;
			}

			// Search Extension List...
			if( m_bExtSearch || ! m_sExtList.empty() )
			{
				uExt = 0;
				while( uExt < uExtCount )
				{
					// Change extension...
					std::string sExt = GetExtension( uExt );
					newFilePath = newFilePath;
					newFilePath.ChangeExtension( sExt );
					if( newFilePath.Exists() ) break;
					uExt++;
				}
				if( uExt < uExtCount ) break;
			}
			else
			{
				if( newFilePath.Exists() ) break;
			}

			// Search Path...
			if( uPath >= uPathCount )
			{
				newFilePath = "";
				return newFilePath;
			}

			newFilePath = GetPath( uPath ) + filePath.Filename();
			uPath++;
		}
	}

	return newFilePath;
}

void ResourceDesc::ExtensionSearch( bool bSearch )
{
	m_bExtSearch = bSearch;
}

bool ResourceDesc::ExtensionSearch()
{
	return m_bExtSearch;
}

// Clears previous paths data.
void ResourceDesc::SetPaths( const std::string & paths )
{
	m_sPathList.clear();
	m_pathList.clear();

	AddPaths( paths );
}

const std::string & ResourceDesc::GetPaths()
{
	return m_sPathList;
}

const std::string & ResourceDesc::GetPath( unsigned int index )
{
	if( index > NumPaths() )
	{
		throw unify::Exception( "QFileDec::GetPath: Index out of range!" );
	}

	return m_pathList[ index ];
}

void ResourceDesc::SetExtensions( const std::string & extensions )
{
	 m_sExtList.clear();
	 m_extensionList.clear();

	 AddExtensions( extensions );
}

const std::string & ResourceDesc::GetExtensions()
{
	return m_sExtList;
}

const std::string & ResourceDesc::GetExtension( unsigned int index )
{
	if( index > NumExtensions() )
	{
		throw unify::Exception( "QFileDec::GetExtension: Index out of range!" );
	}

	return m_extensionList[ index ];
}

unsigned int ResourceDesc::NumPaths()
{
	return (unsigned int)m_pathList.size();
}

unsigned int ResourceDesc::NumExtensions()
{
	return (unsigned int)m_extensionList.size();
}
