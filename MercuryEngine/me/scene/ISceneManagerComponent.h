// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Scene.h>
#include <memory>

namespace me
{
	namespace scene
	{
		/// <summary>
		/// Used to interace with scenes.
		/// </summary>
		class ISceneManagerComponent
		{
		public:
			typedef std::shared_ptr< ISceneManagerComponent > ptr;

			virtual ~ISceneManagerComponent() {}

			virtual const char * GetName() const = 0;

			/// <summary>
			/// Called once when a scene is created.
			/// </summary>
			virtual void OnNewScene( Scene * scene ) = 0;
		};
	}
}