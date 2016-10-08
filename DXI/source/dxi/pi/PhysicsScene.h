// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/Game.h>
#include <dxi/pi/PhysicsInstance.h>
#include <dxi/pi/PhysicsEnvironment.h>
#include <dxi/physics/IScene.h>

namespace dxi
{
	namespace pi
	{
		class PhysicsScene : public physics::IScene
		{
		public:
			PhysicsScene();
			~PhysicsScene();

			void Add( std::shared_ptr< physics::IInstance > object );
            virtual void Add( scene::Object::ptr, const std::string & shape ) {}
			virtual void AddShape( qxml::Element * element ) {}

			void AddEnvironment( PhysicsEnvironment::shared_ptr environment );
			void Update( const RenderInfo & renderInfo, core::IInput & input );
			void Render();
		private:
			unsigned int m_lastTime;
			float m_maxElapsed;
			std::list< std::shared_ptr< physics::IInstance > > m_allObjects;
			std::list< PhysicsEnvironment::shared_ptr > m_environments; // Update to abstract type and list.
		};
	} // namespace pi
} // namespace dxi