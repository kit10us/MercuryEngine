// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/IObjectComponent.h>
#include <me/scene/Object.h>
#include <me/input/IInputSource.h>
#include <me/input/IInputCondition.h>

namespace me
{
	namespace motivator
	{
		///<summary>
		/// Motivates an object to orbit.
		///</summary>
		class ObjectInputMotivator : public scene::IObjectComponent
		{
		public:
			ObjectInputMotivator();

			virtual ~ObjectInputMotivator();

			std::string GetType() const override;
			std::string GetWhat() const override;

			bool IsEnabled() const override;

			void SetEnabled( bool enabled );

			bool Updateable() const override;

			bool Renderable() const override;

			void OnAttach( scene::Object * object ) override;

			void OnDetach() override;

			void OnInit() override;

			void OnStart() override;

			void OnUpdate( UpdateParams params ) override;

			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override;

			void OnSuspend() override;

			void OnResume() override;

			scene::IObjectComponent * Duplicate() override;
		
			void Add( std::string motiviation, input::IInputCondition::ptr condition );

			int GetValueCount() const override;
			bool ValueExists( std::string name ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			std::string GetValue( std::string name ) const override;
			bool SetValue( int index, std::string value ) override;
			bool SetValue( std::string name, std::string value ) override;

		private:
			scene::Object * m_target;
			bool m_enabled;
			std::map< std::string /*motivation*/, input::IInputCondition::ptr, unify::CaseInsensitiveLessThanTest > m_motivations;
			
			float m_walkSpeed;
			float m_runSpeed;
			float m_lookXSpeed;
			float m_lookYSpeed;
		};
	}
}