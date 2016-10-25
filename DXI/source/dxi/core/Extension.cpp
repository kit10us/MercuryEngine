// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Extension.h>
#include <qxml/Document.h>

using namespace dxi;
using namespace core;

typedef bool ( __cdecl *LoaderFunction )(dxi::core::IGame *, const qxml::Document * doc );

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
	m_moduleHandle = LoadLibraryA( m_source.ToString().c_str() );
	if ( ! m_moduleHandle )
	{
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
