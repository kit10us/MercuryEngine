// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <btBulletDynamicsCommon.h>
#include <qxml/Element.h>

namespace bi
{
	bool XMLConvertShape( const qxml::Element * element , boost::shared_ptr< btCollisionShape > & shape );
}