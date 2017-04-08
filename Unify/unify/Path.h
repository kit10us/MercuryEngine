// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <unify/Unify.h>
#include <string>
#include <vector>
#include <iostream>

namespace unify
{
	class Path	
	{
		std::string m_path;
	public:
		Path();
		explicit Path( const std::string & path );
		explicit Path( const char * const path );
		Path( const Path & left, const Path & right );
		Path( const std::vector< std::string > & pathParts );

		bool Empty() const;

		/// <summary>
		/// Splits a path into individual parts. If the path begins with a slash, then the first part will be a slash.
		/// </summary>
		std::vector< std::string > Split() const;

		/// <summary>
		/// Joins a vector of path parts, such as that created by Split, into a single path.
		/// </summary>
		Path & Join( const std::vector< std::string > & pathParts );

		/// <summary>
		/// Attempts to normalize the path, resolving all "..".
		/// </summary>
		Path & Normalize();

		bool Exists() const;
		
		Path DirectoryOnly() const;

		/// <summary>
		/// Returns the extension from a path, includes the leading dot (.).
		/// Returns empty string if no extension was found.
		/// </summary>
		std::string ExtensionOnly() const;

		/// <summary>
		/// Returns the file name (e.g., "c:\folder\filename.ext" ... "filename.ext" )
		/// Equivalent to: boost::filesystem::path.filename
		/// </summary>
		std::string Filename() const;

		/// <summary>
		/// Returns the file name without extension (e.g., "c:\folder\filename.ext" ... "filename" )
		/// Equivalent to: boost::filesystem::path.stem
		/// </summary>
		std::string FilenameNoExtension() const;

		Path & Combine( const Path & left, const Path & right );

		bool IsDirectory() const;

		bool IsExtension( const std::string & extension ) const;

		Path ChangeExtension( std::string extension ) const;

		// Equivalent to Combine( DirectoryOnly( pathIn ), Filename( newFilename ) )
		Path & ChangeFilename( const Path & newFilename );

		Path & operator=( const Path & path );
		Path operator+( const Path & path );
		Path & operator+=( const Path & path );

		std::string ToString() const;
		std::wstring ToWString() const;
	};
}

std::ostream & operator<<( std::ostream & os, const unify::Path & path );
