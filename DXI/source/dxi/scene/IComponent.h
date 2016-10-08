// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace dxi
{
	namespace scene
	{
		class IComponent
		{
		public:
			typedef std::shared_ptr< IComponent > ptr;

			virtual ~IComponent() {}

			virtual std::string GetName() const = 0;
			virtual void SetName( std::string name ) = 0;

			virtual bool GetEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

		private:
			std::string m_name;
			bool m_enabled;
		};
	}
}