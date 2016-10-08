// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/IComponent.h>

namespace dxi
{
	namespace scene
	{
		class Component : public IComponent
		{
		public:
			Component( std::string name );
			~Component();

			std::string GetName() const override;
			void SetName( std::string name ) override;

			bool GetEnabled() const override;
			void SetEnabled( bool enabled ) override;

		private:
			std::string m_name;
			bool m_enabled;
		};
	}
}