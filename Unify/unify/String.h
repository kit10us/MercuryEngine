// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Cast.h>
#include <string>
#include <sstream>
#include <vector>
#include <list>

namespace unify
{
	// Char
	bool IsAlpha( const char ch );
	bool IsNumeric( const char ch );

	// Caseless compare.
	bool StringIs( const std::string & a, const std::string & b );
	bool StringIsAny( const std::string & a, const std::list< std::string > & list );
	bool BeginsWith( const std::string & a, const std::string & b );
	bool EndsWith( const std::string & a, const std::string & b );
	
	bool StringIsInt( const std::string & sOne );
	bool StringIsFloat( const std::string & sOne );

	std::string TrimLeft( std::string stringIn, const char chr );
	std::string TrimRight( std::string stringIn, const char chr );
	std::string Trim( std::string stringIn, const char chr = ' ' );
	std::string RightString( std::string stringIn, std::string::size_type uLength );
	std::string LeftString( std::string stringIn, unsigned int uLength );
	std::string StringMinusLeft( const std::string & sStringIn, unsigned int uLessLength );
	std::string StringMinusRight( const std::string & sStringIn, unsigned int uLessLength );
	
	/// <summary>
	/// Returns a string where all sets (single or in a row) cariage returns, tabs or spaces are replaced with one space.
	/// For example, "\n\n\n  \t\tHello,  \tWorld\n!     \t\n "  >>  "Hello, World!" (note: no leading or training spaces either)
    /// Doesn't reduce ANY single spaces.
	/// </summary>
	std::string CleanWhitespace( const std::string & sIn );

	std::string ToLower( const std::string & in );

	
	// Considers a string as a part of a segmented list by a character, returns that part of the list...
	// Returns ASAP.
	// The first iPartIndex is 0.
	std::string ListPart( const std::string & sString, std::vector< char > seperators, int iPartIndex );	
	unsigned int ListPartCount(const std::string & sString, std::vector< char > seperators );

	// Binary functor for case insensitive string compares. Useful for maps.
    class CaseInsensitiveLessThanTest
    {
    public:
        bool operator() (const std::string & string1, const std::string & string2) const;
    };  

	// TODO: For our needs, multiple spaces as delimitors need to be considered as one.
	template< typename T >
	std::vector< T > Split( const std::string & sourceString, const char delimitor );

	/// <summar>
	/// Returns a vector of whitespace characters including space, newline, and tab.
	std::vector< char > SplitWhitespace();

	/// <summary>
	/// Split a string into a vector of string pieces based on multiple delimitors. includeEmpties effectively includes empty strings if mutliple delimitors
	/// occur in a row. eq. 
	/// </summary>
	/// <example>
	/// "a,,b,c" = ["a", "b", "c" ] { if includeEmtpies = false } else = ["a", "", "b", "c"]
	/// (' ', '\n')"a b   c\n  d" = [ "a", "b", "c", "d" ]{ if includeEmtpies = false } else = ["a", "", "", "b", "c", "" (for '\n'), ""]
	/// </example>
	template< typename T >
	std::vector< T > Split( const std::string & sourceString, const std::vector< char > & delimitors, bool includeEmpties = false );

	template< typename T >
	std::vector< T > SplitOnWhitespace( const std::string & sourceString );

	#include <unify/String.inl>
} // namespace unify
