// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/Extension.h>
#include <me/IGame.h>
#include <qxml/Document.h>

using namespace me;

typedef bool ( __cdecl *LoaderFunction )(me::IGame *, const qxml::Document * doc );

Extension::Extension( unify::Path source )
	: m_source( source )
{
}
							 
Extension::~Extension()
{
	Free();
}

bool Extension::Load( IGame * game )
{
	if ( ! m_source.Exists() )
	{
		game->ReportError( me::ErrorLevel::Critical, "Extension", "Failed to find extenion \"" + m_source.ToString() + "\"!" ); 
	}

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
	
	LoaderFunction loader = (LoaderFunction)GetProcAddress( m_moduleHandle, "DXILoader" );
	if ( ! loader )
	{
		FreeLibrary( m_moduleHandle );
		m_moduleHandle = 0;
		return false;
	}

	unify::Path config( m_source.ChangeExtension( ".xml" ).Filename() );
	std::shared_ptr< qxml::Document > doc;
	if ( config.Exists() )
	{
		doc.reset( new qxml::Document( config ) );
	}

	loader( game, doc.get() );

	return true;
}



void Extension::Free()
{
	FreeLibrary( m_moduleHandle );
	m_moduleHandle = 0;
}
