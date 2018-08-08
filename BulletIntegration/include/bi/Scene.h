// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <bi/Instance.h>
#include <dxi/physics/IScene.h>
#include <bi/DebugDraw.h>
#include <bi/Convert.h>
#include <qxml/Element.h>
#include <dxi/IInput.h>

namespace bi
{
	class Scene : public dxi::physics::IScene
	{
	public:
		Scene();
		~Scene();
		btDiscreteDynamicsWorld * GetWorld();
		const btDiscreteDynamicsWorld * GetWorld() const;

		void AddShape( const std::string & name, boost::shared_ptr< btCollisionShape > shape );
		bool FindShape( const std::string & name, boost::shared_ptr< btCollisionShape > & shape );
		void Add( boost::shared_ptr< dxi::physics::IInstance > object );
		void Add( dxi::scene::Object::shared_ptr object, const std::string & shapeName );
		void Update( unify::Seconds elapsed, dxi::IInput & input );
		void Render();

	private:
		unsigned int m_lastTime;
		boost::shared_ptr< DebugDraw > m_debugDraw;
		boost::shared_ptr< btBroadphaseInterface > m_broadphase;
		boost::shared_ptr< btDefaultCollisionConfiguration > m_collisionConfiguration;
		boost::shared_ptr< btCollisionDispatcher > m_dispatcher;
		boost::shared_ptr< btSequentialImpulseConstraintSolver > m_solver;
		boost::shared_ptr< btDiscreteDynamicsWorld > m_dynamicsWorld;
		boost::shared_ptr< btCollisionShape > m_groundShape;
		boost::shared_ptr< btDefaultMotionState > m_groundMotionState;
		boost::shared_ptr< btRigidBody > m_groundRigidBody;

		std::map< std::string, boost::shared_ptr< btCollisionShape > > m_shapes;
		std::list< boost::shared_ptr< dxi::physics::IInstance > > m_allObjects;
	};
} // using namespace bi