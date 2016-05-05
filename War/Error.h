#pragma once

#include <dxerr.h>
#include <string>
#include <exception>

#pragma comment( lib, "dxerr.lib" )

// Preprocessor define instead of function to prevent initialization of 'message'.
#define OnFailedThrow( hr, message ) \
	if ( FAILED( hr ) ) { \
		std::string what( message ); \
		what += std::string( "\n[" ) + DXGetErrorStringA( hr ) + std::string( "]" ); \
		throw std::exception( what.c_str() ); \
	}

#define OnFalseThrow( result, message ) \
	if ( ! ( result ) ) throw std::exception( std::string( message ).c_str() );

#define OnFalseGetLastErrorThrow( result, message ) \
	if ( ! result ) \
	{ \
		LPSTR messageBuffer = 0; \
		FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPSTR)&messageBuffer, 0, 0 ); \
		std::string errorString( message ); \
		errorString += std::string( "\n" ) + messageBuffer; \
		LocalFree( messageBuffer ); \
		throw std::exception( errorString.c_str() ); \
	}
