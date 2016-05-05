// Copyright (c) 2003 - 2014, Quentin S. Smith
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
		Path( const std::string & path );
		Path( const char * const path );
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

		bool IsExtension( const std::string & extension ) const;

		Path & ChangeExtension( const std::string & extension );

		// Equivalent to Combine( DirectoryOnly( pathIn ), Filename( newFilename ) )
		Path & ChangeFilename( const Path & newFilename );

		Path & operator=( const Path & path );
		Path operator+( const Path & path );
		Path & operator+=( const Path & path );

		std::string ToString() const;
		std::wstring ToWString() const;
	};

	/*
	namespace file
	{
		// Used to help auto-documenting type requirements.
		typedef std::string Path;

		/// <summary>
		/// Splits a path into individual parts. If the path begins with a slash, then the first part will be a slash.
		/// </summary>
		std::vector< std::string > Split( const Path & path );

		/// <summary>
		/// Joins a vector of path parts, such as that created by Split, into a single path.
		/// </summary>
		Path Join( const std::vector< std::string > & pathParts );

		/// <summary>
		/// Attempts to normalize the path, resolving all "..".
		/// </summary>
		Path Normalize( const Path & path );

		bool Exists( const Path & path );
		
		Path DirectoryOnly( const Path & pathIn );

		/// <summary>
		/// Returns the extension from a path, includes the leading dot (.).
		/// Returns empty string if no extension was found.
		/// </summary>
		std::string ExtensionOnly( const Path & pathIn );

		// Returns the file name (e.g., "c:\folder\filename.ext" ... "filename.ext" )
		// Equivalent to: boost::filesystem::path.filename
		std::string Filename( const Path & pathIn );

		// Returns the file name without extension (e.g., "c:\folder\filename.ext" ... "filename" )
		// Equivalent to: boost::filesystem::path.stem
		std::string FilenameNoExtension( const Path & pathIn );

		Path Combine( const Path & leftPath, const Path & rightPath );

		bool IsExtension( const Path & path, const std::string & extension );

		std::string ChangeExtension( const Path & pathIn, const std::string & extension );

		// Equivalent to Combine( DirectoryOnly( pathIn ), Filename( newFilename ) )
		std::string ChangeFilename( const Path & pathIn, const Path & newFilename );

		std::string ToString( const Path & path );
	}
	*/
}

std::ostream & operator<<( std::ostream & os, const unify::Path & path );
