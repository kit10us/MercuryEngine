// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/IObjectComponent.h>
#include <me/scene/Object.h>
#include <me/motivator/IMotivator.h>
#include <me/input/IInputSource.h>
#include <me/input/IInputCondition.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class ObjectInputMotivator : public scene::IObjectComponent, IMotivator
		{
		public:
			ObjectInputMotivator();

			virtual ~ObjectInputMotivator();

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
		
			void Add( std::string motiviation, input::IInputCondition::ptr condition );

		private:
			scene::Object * m_target;
			bool m_enabled;
			std::map< std::string /*motivation*/, input::IInputCondition::ptr, unify::CaseInsensitiveLessThanTest > m_motivations;
		};
	}
}