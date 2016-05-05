// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/physics/IInstance.h>
#include <unify/Frame.h>
#include <unify/BBox.h>
#include <unify/BSphere.h>
#include <dxi/physics/IObjectSyncer.h>

namespace dxi
{
	namespace pi
	{
		// Data modifiers for instancing of a specific physics unit (object).
		class PhysicsInstance : public physics::IInstance
		{
		public:
			PhysicsInstance( physics::IObjectSyncer * syncer );
			~PhysicsInstance();

			// from physics::IInstance...
			void SyncObjectFromPhysics();
			void SyncPhysicsFromObject();
			bool IsActive() const;


			void SetBBox( unify::BBox< float > * bbox );
			void SetMoveable( bool moveable );
			bool GetMoveable() const;
			void SetPhysical( bool physical );
			bool GetPhysical() const;
			unify::Frame * GetFrame();
			unify::Frame * GetPreviousFrame();
			void StopMomentum();
			bool IsUpdating() const;
			
			void BeginUpdate();
			void ApplyForce( const unify::V3< float > & force );
			void CaptureTotalForce();
			void MunchAndPredict();
			bool StillSomeForce() const;
			void Collide( PhysicsInstance * other );
			void EndUpdate();

		private:
			physics::IObjectSyncer * m_syncer;
			unify::Frame m_frame;
			unify::BBox< float > m_bbox;
			unify::Frame m_previousFrame;
			unify::Frame m_tempFrame;

			// Characteristics
			bool m_moveable; // True if object can be affected by environment, and other objects.
			bool m_physical; // True if object can affect other objects.
			bool m_isUpdating;
			unify::V3< float > m_momentum; // Accumilation of forces.
			unify::V3< float > m_currentMomentum;
		};
	} // namespace pi
} // namespace dxi