// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/ShapeFactory.h>
#include <dxi/exception/FailedToCreate.h>

using namespace dxi;

Geometry * ShapeJsonFactory::Produce( qjson::Object json )
{
	return shapes::CreateShape( json );
}
