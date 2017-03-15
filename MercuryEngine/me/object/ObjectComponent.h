// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/object/IObjectComponent.h>
#include <me/IOS.h>

namespace me
{
	namespace object
	{
		class ObjectComponent : public IObjectComponent
		{
		protected:
			ObjectComponent( const ObjectComponent & component );

			Lookup * GetLookup();
			const Lookup * GetLookup() const;

		public:
			ObjectComponent( std::string type );
			~ObjectComponent();

			Object* GetObject();
			const Object* GetObject() const;

		protected:
			void AddInterface( std::string name, IUnknown* ptr );

		public:	 // IObjectComponent...
			bool Updateable() const override { return false; }
			bool Renderable() const override { return false; }

			void OnAttach( Object * object ) override;
			void OnDetach( Object * object ) override;
			void OnInit() override;
			void OnStart() override;
			void OnUpdate( UpdateParams params ) override;
			void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) override;
			void OnSuspend() override;
			void OnResume() override;

		public:	// IComponent...
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


		public: // IUnknown...
			std::string GetTypeName() const override;
			IUnknown* QueryInterface( std::string name ) override;

		protected:
			object::Object * m_object;
			std::string m_typeName;
			bool m_enabled;
			Lookup m_values;
			std::map< std::string, IUnknown*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}