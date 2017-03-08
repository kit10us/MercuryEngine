// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/Extension.h>
#include <me/IGame.h>
#include <qxml/Document.h>

using namespace me;

typedef bool ( __cdecl *LoaderFunction )(me::IGame *, const qxml::Element * element );

Extension::Extension( unify::Path source )
	: m_source( source )
	, m_moduleHandle( 0 )
{
}
							 
Extension::~Extension()
{
	Free();
}

bool Extension::Load( IGame * game, const qxml::Element * element )
{
	if ( ! m_source.Exists() )
	{
		game->ReportError( me::ErrorLevel::Critical, "Extension", "Failed to find extenion \"" + m_source.ToString() + "\"!" ); 
	}

	game->LogLine( "Loading extenion \"" + m_source.ToString() + "\"..." );

	m_moduleHandle = LoadLibraryA( m_source.ToString().c_str() );
	if ( ! m_moduleHandle )
	{
		DWORD errorCode = GetLastError();
		if ( errorCode == ERROR_MOD_NOT_FOUND )
		{
			game->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured due to likely missing dependency (missing another DLL)!" );
		}
		else
		{
			game->ReportError( ErrorLevel::Critical, "Extension", "Extension, \"" + m_source.ToString() + "\" loaded, however, a failure occured (error code: " + unify::Cast< std::string >( errorCode ) + ")!" );
		}
		return false;
	}
	
	LoaderFunction loader = (LoaderFunction)GetProcAddress( m_moduleHandle, "MELoader" );
	if ( ! loader )
	{
		FreeLibrary( m_moduleHandle );
		m_moduleHandle = 0;
		return false;
	}

	loader( game, element );

	return true;
}											

void Extension::Free()
{
	FreeLibrary( m_moduleHandle );
	m_moduleHandle = 0;
}
