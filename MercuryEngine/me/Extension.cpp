// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/Extension.h>
#include <me/game/IGame.h>
#include <me/debug/Block.h>
#include <me/exception/FileNotFound.h>
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
	debug::Block( gameInstance->GetOS()->Debug(), "Extension::Create( \"" + source.ToString() + "\" )" );
	m_source = source;
	gameInstance->GetOS()->Debug()->Try( [&]
	{
		if( !m_source.Exists() )
		{
			throw exception::FileNotFound( m_source );
		}
	}, ErrorLevel::Extension, false, true );

	gameInstance->GetOS()->Debug()->Try( [&]
	{
		m_moduleHandle = LoadLibraryA( m_source.ToString().c_str() );
		if( !m_moduleHandle )
		{
			DWORD errorCode = GetLastError();
			if( errorCode == ERROR_MOD_NOT_FOUND )
			{
				throw unify::Exception( "Extension \"" + m_source.ToString() + "\" loaded, however, a failure occured due to likely missing dependency (missing another DLL)!" );
			}
			else
			{
				throw unify::Exception( "Extension \"" + m_source.ToString() + "\" loaded, however, a failure occured (error code: " + unify::Cast< std::string >( errorCode ) + ")!" );
			}
		}
	}, ErrorLevel::Extension, false, true );

	LoaderFunction loader{};
	{
		debug::Block block( gameInstance->Debug(), "MELoader" );
		loader = (LoaderFunction)GetProcAddress( (HMODULE)m_moduleHandle, "MELoader" );

		gameInstance->Debug()->Try( [&]
		{ 
			if( !loader )
			{
				FreeLibrary( (HMODULE)m_moduleHandle );
				m_moduleHandle = 0;
				throw unify::Exception( "Extension, \"" + m_source.ToString() + "\" loaded, however MELoader not found!" );
			}
		}, ErrorLevel::Extension, false, false );

		gameInstance->Debug()->Try( [&]
		{
			loader( gameInstance, element );
		}, ErrorLevel::Extension, false, false );
	}
}
