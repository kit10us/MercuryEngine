// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <string>

///<summary>
/// Parsing relies heavily on the ability to cast to-and-from strings. This
/// library helps facilitate such a feature.
///</summary>

namespace unify
{
	// The default cast will throw (not a supported cast).
	template< typename TTo, class TFrom > TTo Cast( const TFrom in );

	// From X to std::string...
	template<> std::string Cast( const std::string in );
	template<> std::string Cast( const bool in );
	template<> std::string Cast( const unsigned char in );
	template<> std::string Cast( const char in );
	template<> std::string Cast( const unsigned int in );
	template<> std::string Cast( const int in );
	template<> std::string Cast( const float in );
	template<> std::string Cast( const double in );
	template<> std::string Cast( const unsigned short in );
	template<> std::string Cast( const short in );

	// From std::string to X...
	template<> bool Cast( const std::string text );
	template<> char Cast( const std::string text );
	template<> unsigned char Cast( const std::string text );
	template<> int Cast( const std::string text );
	template<> unsigned int Cast( const std::string text );
	template<> float Cast( const std::string text );
	template<> double Cast( const std::string text );
	template<> wchar_t * Cast( const std::string text );
	template<> short Cast( const std::string text );
	template<> unsigned short Cast( const std::string text );
	template<> std::wstring Cast( const std::string text );

	template<> bool Cast( const char * text );
	template<> char Cast( const char * text );
	template<> unsigned char Cast( const char * text );
	template<> int Cast( const char * text );
	template<> unsigned int Cast( const char * text );
	template<> float Cast( const char * text );
	template<> double Cast( const char * text );
	template<> wchar_t * Cast( const char * text );

}