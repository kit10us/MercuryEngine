// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/Extension.h>
#include <me/game/IGame.h>
#include <qxml/Document.h>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#ifdef GetCommandLine
#undef GetCommandLine
#endif // GetCommandLine
#ifdef GetObject
#undef GetObject
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif


using namespace me;

typedef bool ( __cdecl *LoaderFunction )( game::IGame *, const qxml::Element * element );

Extension::Extension( unify::Path source )
	: m_source( source )
	, m_moduleHandle( 0 )
{
}
							 
Extension::~Extension()
{
	Free();
}

bool Extension::Load( game::IGame * gameInstance, const qxml::Element * element )
{
	auto * debug = gameInstance->Debug();

	if ( ! m_source.Exists() )
	{
		if( debug )
		{
			debug->ReportError( me::ErrorLevel::Critical, "Extension", "Failed to find extenion \"" + m_source.ToString() + "\"!" );
		}
	}

	if( debug )
	{
		debug->LogLine( "Extension::Load", "Loading extenion \"" + m_source.ToString() + "\"..." );
	}

	m_moduleHandle = LoadLibraryA( m_source.ToString().c_str() );
	if ( ! m_moduleHandle )
	{
		DWORD errorCode = GetLastError();
		if ( errorCode == ERROR_MOD_NOT_FOUND )
		{
			gameInstance->Debug()->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured due to likely missing dependency (missing another DLL)!" );
		}
		else
		{
			gameInstance->Debug()->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured (error code: " + unify::Cast< std::string >( errorCode ) + ")!" );
		}
		return false;
	}
	
	LoaderFunction loader = (LoaderFunction)GetProcAddress( (HMODULE)m_moduleHandle, "MELoader" );
	if ( ! loader )
	{
		FreeLibrary( (HMODULE)m_moduleHandle );
		m_moduleHandle = 0;
		return false;
	}

	loader( gameInstance, element );

	return true;
}											

void Extension::Free()
{
	FreeLibrary( (HMODULE)m_moduleHandle );
	m_moduleHandle = 0;
}
