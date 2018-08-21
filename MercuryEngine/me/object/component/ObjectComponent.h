// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/object/component/IObjectComponent.h>
#include <me/object/Object.h>

namespace me
{
	namespace object
	{
		namespace component
		{
			class ObjectComponent : public IObjectComponent
			{
			protected:
				ObjectComponent( const ObjectComponent & component );

				Lookup * GetLookup();
				const Lookup * GetLookup() const;

			public:
				ObjectComponent( std::string type, bool update, bool render );
				~ObjectComponent();


			protected:
				void AddInterface( std::string name, me::IThing* ptr );

			public:	 // IObjectComponent...
				void SetAlias( std::string alias ) override;
				std::string GetAlias() const override;

				Object* GetObject();
				const Object* GetObject() const;

				void GetBBox( unify::BBox< float > & bbox, const unify::Matrix & matrix ) const override;
				bool Updateable() const override;
				bool Renderable() const override;
				void OnAttach( Object * object ) override;
				void OnDetach( Object * object ) override;
				void OnStart() override;
				void OnUpdate( const UpdateParams & params ) override;
				void CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans, const unify::FrameLite * frame ) override;
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
				bool SetValue( int index, std::string value ) override;

				// Help functions...
				std::string GetValue( std::string name ) const override;
				bool SetValue( std::string name, std::string value ) override;


			public: // me::IThing...
				std::string GetTypeName() const override;
				me::IThing* QueryInterface( std::string name ) override;
				std::string GetWhat() const override;

			protected:
				object::Object * m_object;
				std::string m_typeName;
				std::string m_alias;
				bool m_enabled;
				bool m_update;
				bool m_render;
				Lookup m_values;
				std::map< std::string, me::IThing*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
			};
		}
	}
}