// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/IObjectComponent.h>
#include <me/IOS.h>

namespace me
{
	namespace scene
	{
		class ObjectComponent : public IObjectComponent
		{
		protected:
			ObjectComponent( ObjectComponent & component );

			Lookup * GetLookup();
			const Lookup * GetLookup() const;

		public:
			ObjectComponent( IOS * os );
			~ObjectComponent();

			IOS * GetOS();
			const IOS * GetOS() const;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			int GetValueCount() const override;
			bool ValueExists( std::string ) const override;
			std::string GetValueName( int index ) const override;
			int FindValueIndex( std::string name ) const override;
			std::string GetValue( int index ) const override;
			std::string GetValue( std::string name ) const override;
			bool SetValue( int index, std::string value ) override;
			bool SetValue( std::string name, std::string value ) override; 

		public:	   

			bool Updateable() const override { return false; }
			bool Renderable() const override { return false; }			   
										   
			void OnAttach( Object * object ) override {}
			void OnDetach() override {}
			void OnInit() override {}
			void OnStart() override {}
			void OnUpdate( UpdateParams params ) override {}
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override {}
			void OnSuspend() override {}
			void OnResume() override {}

		protected:
			IOS * m_os;
			bool m_enabled;
			Lookup m_values;
		};
	}
}