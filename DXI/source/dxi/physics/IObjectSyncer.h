// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/BBox.h>
#include <unify/Frame.h>

namespace dxi
{
	namespace physics
	{
		class IObjectSyncer
		{
		public:
			IObjectSyncer() {}
			virtual ~IObjectSyncer() {}

			virtual const unify::Frame & GetFrame() = 0;
			virtual const unify::BBox< float > & GetBBox() = 0;

			virtual void SyncFrame( const unify::Frame & frame ) = 0;
			virtual void SyncBBox( const unify::BBox< float > & bbox ) = 0;
			virtual void SyncJoints() {} // TODO: ?
		};
	} // namespace physics
} // namespace 