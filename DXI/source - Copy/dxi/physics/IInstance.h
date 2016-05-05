// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace dxi
{
	namespace physics
	{
		class IInstance
		{
		public:
			IInstance() {}
			virtual ~IInstance() {}
			virtual void SyncObjectFromPhysics() = 0;
			virtual void SyncPhysicsFromObject() = 0;
			virtual bool IsActive() const = 0;
		};
	}
}