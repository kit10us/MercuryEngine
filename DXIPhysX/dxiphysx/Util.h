// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>
#include <unify/V3.h>
#include <unify/Matrix.h>

namespace util
{	

	template< typename TTo, typename TFrom >
	TTo Convert( const TFrom & from )
	{
		return *(TTo*)(&from);
	}

	template<>
	physx::PxMat44 Convert( const unify::Matrix & matrix );

	template<>
	physx::PxVec3 Convert( const unify::V3< float > & v );

	template<>
	physx::PxTransform Convert( const unify::Matrix & matrix );

	template<>
	physx::PxPlane Convert( const unify::Matrix & matrix );

	template<>
	unify::Matrix Convert( const physx::PxMat44 & matrix );

	template<>
	unify::V3< float > Convert( const physx::PxVec3 & v );
}
