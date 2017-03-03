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

			int GetValueCount() const override;
			bool ValueExists( std::string name ) const override;
			std::string GetValueName( size_t index ) const override;
			int FindValueIndex( std::string name ) const override;
			void SetValue( size_t index, std::string value ) override;
			void SetValue( std::string name, std::string value ) override;
			std::string GetValue( size_t index ) const override;
			std::string GetValue( std::string name ) const override;

		private:
			scene::Object * m_target;
			bool m_enabled;
			unify::V3< float > m_origin;
			unify::V3< float > m_orbit;
			unify::Angle m_angleASecond;
		};
	}
}