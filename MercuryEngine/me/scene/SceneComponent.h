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

		public: // ISceneComponent...
			void OnAttach( Scene * scene ) override;
			void OnDetach( Scene * scene ) override;
			void OnInit( Scene * scene ) override;
			void OnStart( Scene * scene ) override;
			void OnUpdate( Scene * scene, UpdateParams params ) override;
			void OnRender( Scene * scene, RenderParams params ) override;
			void OnSuspend() override;
			void OnResume() override;

		public:	// IComponent...
			std::string GetTypeName() const override;
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

		private:
			IOS * m_os;
			std::string m_typeName;
			bool m_enabled;
			Lookup m_values;
		};
	}
}