// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <btBulletDynamicsCommon.h>
#include <unify/V3.h>
#include <unify/Matrix.h>
#include <unify/Quaternion.h>
#include <unify/Frame.h>


namespace bi
{
	btVector3 convert( const unify::V3< float > & in );
	unify::V3< float > convert( const btVector3 & in );
	btQuaternion convert( const unify::Quaternion & in );
	unify::Quaternion convert( const btQuaternion & in );
	btMatrix3x3 convert( const unify::Matrix & in );
	btTransform convert( const unify::Frame & in );
	unify::Frame convert( const btTransform & in );
}