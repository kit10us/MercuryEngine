// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/Util.h>

template<>
physx::PxMat44 util::Convert( const unify::Matrix & matrix )
{
	return *(physx::PxMat44*)&matrix;
}

template<>
physx::PxVec3 util::Convert( const unify::V3< float > & v )
{
	return *(physx::PxVec3*)&v;
}

template<>
physx::PxTransform util::Convert( const unify::Matrix & matrix )
{
	return physx::PxTransform(*(physx::PxMat44*)&matrix);
}

template<>
physx::PxPlane util::Convert( const unify::Matrix & matrix )
{
	return physx::PxPlane( Convert< physx::PxVec3 >( matrix.GetPosition() ), Convert< physx::PxVec3 >( matrix.GetUp() ) );
}

template<>
unify::Matrix util::Convert( const physx::PxMat44 & matrix )
{
	return *(unify::Matrix*)&matrix;
}

template<>
unify::V3< float > util::Convert( const physx::PxVec3 & v )
{
	return *(unify::V3< float >*)&v;
}
