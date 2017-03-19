// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>
#include <me/scene/IScene.h>
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

			IScene* GetScene();
			const IScene* GetScene() const;

		protected:
			void AddInterface( std::string name, IUnknown* ptr );

		public: // ISceneComponent...
			void OnAttach( me::scene::IScene * scene ) override;
			void OnDetach( me::scene::IScene * scene ) override;
			void OnEnterScene( me::scene::IScene * previous ) override;
			void OnLeaveScene( me::scene::IScene * next ) override;
			void OnBeforeStart() override;
			void OnAfterStart() override;
			void OnUpdate( UpdateParams params ) override;
			void CollectCameras( RenderGirl & renderGirl ) override;
			void OnRender( RenderGirl & renderGirl ) override;
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
			IScene* m_scene;
			Lookup m_values;
			std::map< std::string, IUnknown*, unify::CaseInsensitiveLessThanTest > m_interfaceMap;
		};
	}
}