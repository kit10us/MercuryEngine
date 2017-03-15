// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/IOS.h>

namespace me
{
	namespace scene
	{
		class SceneComponent : public ISceneComponent
		{
		public:
			SceneComponent( IOS * os, std::string typeName );
			~SceneComponent();

			IOS * GetOS();
			const IOS * GetOS() const;

		protected:
			void AddInterface( std::string name, IUnknown* ptr );

		public: // ISceneComponent...
			void OnAttach( Scene * scene ) override;
			void OnDetach( Scene * scene ) override;
			void OnInit( Scene * scene ) override;
			void OnStart( Scene * scene ) override;
			void OnUpdate( Scene * scene, UpdateParams params ) override;
			void OnRender( Scene * scene, RenderGirl & renderGirl ) override;
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

		public:
			std::string GetTypeName() const override;
			IUnknown* QueryInterface( std::string name ) override;

		private:
			IOS * m_os;
			std::string m_typeName;
			bool m_enabled;
			Lookup m_values;
			std::map< std::string, IUnknown*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}