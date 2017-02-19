// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/IObjectComponent.h>
#include <me/scene/Object.h>
#include <me/motivator/IMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class ObjectOrbitMotivator : public scene::IObjectComponent, IMotivator
		{
		public:
			ObjectOrbitMotivator( unify::V3< float > origin, unify::V3< float > orbit, unify::Angle angleASecond );

			virtual ~ObjectOrbitMotivator();

			std::string GetName() const override;

			bool IsEnabled() const override;

			void SetEnabled( bool enabled );

			bool Updateable() const override;

			bool Renderable() const override;

			void OnAttach( scene::Object * object ) override;

			void OnDetach() override;

			void OnInit() override;

			void OnStart() override;

			void OnUpdate( IRenderer * renderer, const RenderInfo & renderInfo ) override;

			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override;

			void OnSuspend() override;

			void OnResume() override;

			scene::IObjectComponent * Duplicate() override;

			void SetOrigin( unify::V3< float > origin );
			
			void SetOrbit( unify::V3< float > orbit );
			
			void SetAngleASecond( unify::Angle angleASecond );
			
			unify::V3< float > GetOrigin() const;

			unify::V3< float > GetOrbit() const;

			unify::Angle GetAngleASecond() const;

		private:
			scene::Object * m_target;
			bool m_enabled;
			unify::V3< float > m_origin;
			unify::V3< float > m_orbit;
			unify::Angle m_angleASecond;
		};
	}
}