// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#include <bi/Convert.h>

using namespace bi;

btVector3 bi::convert( const unify::V3< float > & in )
{
	return btVector3( in.x, in.y, in.z );
}

unify::V3< float > bi::convert( const btVector3 & in )
{
	return unify::V3< float >( in.x(), in.y(), in.z() );
}

btQuaternion bi::convert( const unify::Quaternion & in )
{
	btQuaternion out( in.GetX(), in.GetY(), in.GetZ(), in.GetW() );
	return out;
}

unify::Quaternion bi::convert( const btQuaternion & in )
{
	unify::Quaternion out( in.x(), in.y(), in.z(), in.w() );
	return out;
}

btMatrix3x3 bi::convert( const unify::Matrix & in )
{
	btMatrix3x3 out;
	out[ 0 ] = bi::convert( in.GetLeft() );
	out[ 1 ] = bi::convert( in.GetUp() );
	out[ 2 ] = bi::convert( in.GetForward() );
	return out;
}

btTransform bi::convert( const unify::Frame & in )
{
	btTransform out( bi::convert( in.GetLocalMatrix() ), bi::convert( in.GetPosition() ) );
	return out;
}

unify::Frame bi::convert( const btTransform & in )
{
	btMatrix3x3 matrix;
	matrix = in.getBasis().inverse();
	unify::Frame out;
	out.SetLeft( bi::convert( matrix[ 0 ] ) );
	out.SetUp( bi::convert( matrix[ 1 ] ) );
	out.SetForward( bi::convert( matrix[ 2 ] ) );
	out.SetPosition( bi::convert( in.getOrigin() ) );
	return out;
}