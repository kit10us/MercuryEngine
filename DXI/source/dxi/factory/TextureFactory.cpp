// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/TextureFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

Texture * TextureSourceFactory::Produce( unify::Path source )
{
	if ( ! source.Empty() )
	{
		return new Texture( source );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}

Texture * TextureJsonFactory::Produce( qjson::Object json )
{
	unify::Path source( json[ "source" ].ToString() );

	unsigned int flags = 0;
	if( json.Has( "lockable" ) )
	{
		flags |= TEXTURE_LOCKABLE;
	}
	return new Texture( source, flags );
}

Texture * TextureXmlNodeFactory::Produce( const qxml::Element & node )
{
	unify::Path source( unify::Path( node.GetDocument()->GetPath() ).DirectoryOnly() + node.GetAttribute< std::string >( "source" ) );

	unsigned int flags = 0;
	if( node.HasAttributes( "lockable" ) )
	{
		flags |= TEXTURE_LOCKABLE;
	}
	return new Texture( source, flags );
}