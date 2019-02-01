// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/Extension.h>
#include <me/game/IGame.h>
#include <me/debug/Block.h>
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

Extension::Extension()
	: m_moduleHandle{ nullptr }
{
}
							 
Extension::~Extension()
{
	FreeLibrary( (HMODULE)m_moduleHandle );
	m_moduleHandle = 0;
}

void Extension::Create( game::IGame * gameInstance, unify::Path source, const qxml::Element * element )
{
	m_source = source;
	if( !m_source.Exists() )
	{
		gameInstance->Debug()->ReportError( me::ErrorLevel::Critical, "Extension::Load", "Failed to find extenion \"" + m_source.ToString() + "\"!" );
	}

	m_moduleHandle = LoadLibraryA( m_source.ToString().c_str() );
	if( !m_moduleHandle )
	{
		DWORD errorCode = GetLastError();
		if( errorCode == ERROR_MOD_NOT_FOUND )
		{
			gameInstance->Debug()->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured due to likely missing dependency (missing another DLL)!" );
		}
		else
		{
			gameInstance->Debug()->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured (error code: " + unify::Cast< std::string >( errorCode ) + ")!" );
		}
	}

	LoaderFunction loader{};
	{
		debug::Block block( gameInstance->Debug(), "MELoader" );
		loader = (LoaderFunction)GetProcAddress( (HMODULE)m_moduleHandle, "MELoader" );

		if( !loader )
		{
			FreeLibrary( (HMODULE)m_moduleHandle );
			m_moduleHandle = 0;
			gameInstance->Debug()->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however MELoader not found!" );
		}
		loader( gameInstance, element );
	}
}
