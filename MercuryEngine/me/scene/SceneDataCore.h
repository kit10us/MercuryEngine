// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneComponent.h>

namespace me
{
	namespace scene
	{
		class IModifier
		{
		public:
		};

		class ICondition
		{
		public:
		};

		namespace condition
		{

		}

		/*
		A

		*/




		/// <summary> 
		/// Game data manager.
		/// </summary>
		class SceneDataCore : public ISceneComponent
		{
		public:
			SceneDataCore();
			virtual ~SceneDataCore();

			bool IsEnabled() const override;

			void SetEnabled( bool enabled ) override;

			void OnAttach( Scene * scene ) override;

			void OnDetach( Scene * scene ) override;

			void OnInit( Scene * scene ) override;

			void OnStart( Scene * scene ) override;

			void OnUpdate( Scene * scene, UpdateParams params ) override ;

			void OnRender( Scene * scene, RenderParams params ) override;

			void OnSuspend() override;

			void OnResume() override;

		private:
			// On_Condition Do_Action Return_Set
		};
	}
}