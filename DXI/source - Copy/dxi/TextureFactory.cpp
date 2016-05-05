// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
//

#include <dxi/TextureFactory.h>

using namespace dxi;

Texture * TextureFactory::Produce( const qjson::Object & object )
{
	return new Texture( object["path"]->ToString() );
}
