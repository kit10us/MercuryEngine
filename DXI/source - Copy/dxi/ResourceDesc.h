// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
//
///<summary>
/// Describes a file/resource type - specifying common paths to the resource (for omission), also
/// descrives common extensions.
///</summary>

#pragma once

#include <unify/Unify.h>
#include <vector>
#include <unify/Path.h>

namespace dxi 
{
	class ResourceDesc
	{
	public:
		ResourceDesc();
		ResourceDesc( const ResourceDesc & fileDesc );
		ResourceDesc( const std::string & sPaths, const std::string & sExts );
		~ResourceDesc();

		void AddPaths( const std::string & paths );
		void SetPaths( const std::string & paths );
		const std::string & GetPaths();
		const std::string & GetPath( unsigned int index );
		void AddExtensions( const std::string & extensions );
		void SetExtensions( const std::string & extensions );
		const std::string & GetExtensions();
		const std::string & GetExtension( unsigned int index );
		bool HasExtension( const std::string & extension );
		void ExtensionSearch( bool bSearch );
		bool ExtensionSearch();
		unify::Path CreateFilepath( const unify::Path & filePath );
		unsigned int NumPaths();
		unsigned int NumExtensions();

	protected:
		std::string m_sPathList;
		std::vector< std::string > m_pathList;
		std::string m_sExtList;
		std::vector< std::string > m_extensionList;
		bool m_bExtSearch;
	};
}

