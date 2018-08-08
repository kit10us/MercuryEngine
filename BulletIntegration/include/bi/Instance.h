// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <bi/Convert.h>
#include <dxi/physics/IInstance.h>
#include <dxi/physics/IObjectSyncer.h>
#include <boost/shared_ptr.hpp>

namespace bi
{
	class Instance : public dxi::physics::IInstance, public btMotionState
	{
	public:
		Instance( dxi::physics::IObjectSyncer * syncer, boost::shared_ptr< btCollisionShape > collisionShape );

		// from dxi::physics::IInterface...
		void SyncObjectFromPhysics();
		void SyncPhysicsFromObject();
		bool IsActive() const;

		void SetShape( boost::shared_ptr< btCollisionShape > shape );
		void SetRigidBody( boost::shared_ptr< btRigidBody > rigidBody );
		btRigidBody * GetRigidBody();
		const btRigidBody * GetRigidBody() const;

		void getWorldTransform( btTransform & worldTrans ) const;
		void setWorldTransform( const btTransform & worldTrans );

	private:
		dxi::physics::IObjectSyncer * m_syncer;
		boost::shared_ptr< btCollisionShape > m_shape;
		boost::shared_ptr< btRigidBody > m_rigidBody;
	};
}