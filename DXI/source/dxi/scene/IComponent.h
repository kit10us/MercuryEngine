// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderInfo.h>

#include <string>
#include <memory>

namespace dxi
{
	namespace scene
	{
		class Object;

		class IComponent
		{
		public:
			typedef std::shared_ptr< IComponent > ptr;

			virtual ~IComponent() {}

			virtual std::string GetName() const = 0;
			virtual void SetName( std::string name ) = 0;

			virtual bool IsEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

			/// <summary>
			/// Called once, regardless of enabled or not, before all other events.
			/// </summary>
			virtual void OnInit( Object * object ) = 0;

			/// <summary>
			/// called once, when enabled, only immediatly before the first OnUpdate.
			/// </summary>
			virtual void OnStart( Object * object ) = 0;

			/// <summary>
			/// Called every game update cycle.
			/// </summary>
			virtual void Update( const RenderInfo & renderInfo ) = 0;

			/// <summary>
			/// Called during rendering.
			/// </summary>
			virtual void Render( const RenderInfo & renderInfo ) = 0;

			/// <summary>
			/// Called when we get a suspend request. One more Render is called to allow updating based on becoming suspend (suspended graphics).
			/// </summary>
			virtual void OnSuspend() = 0;

			/// <summary>
			/// Called when resuming from suspending.
			/// </summary>
			virtual void OnResune() = 0;

		private:
			std::string m_name;
			bool m_enabled;
		};
	}
}