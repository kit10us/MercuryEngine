// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/TextureFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

Texture * TextureSourceFactory::Produce( unify::Path source )
{
	if( !source.Empty() )
	{
		return new Texture( source );
	}

	throw exception::FailedToCreate( "No valid factory option provided for texture!" );
}

Texture * TextureXmlNodeFactory::Produce( const qxml::Element & node )
{
	unify::Path source( unify::Path( node.GetDocument()->GetPath() ).DirectoryOnly() + node.GetAttribute< std::string >( "source" ) );

	bool lockable = false;
	bool renderable = true;
	renderable = node.GetAttributeElse< bool >( "renderable", true );
	lockable = node.GetAttributeElse< bool >( "lockable", false );
	return new Texture( source, renderable, lockable );
}