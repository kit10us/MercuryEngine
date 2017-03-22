// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectInputMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

ObjectInputMotivator::ObjectInputMotivator( const ObjectInputMotivator & objectInputMotivator )
	: ObjectComponent( objectInputMotivator  )
{
}


ObjectInputMotivator::ObjectInputMotivator()
	: ObjectComponent( "ObjectInputMotivator", true, false )
{
}

ObjectInputMotivator::~ObjectInputMotivator() 
{
}

motivator::IInputMotivator * ObjectInputMotivator::GetMotivator()
{
	return &m_motivator;
}
