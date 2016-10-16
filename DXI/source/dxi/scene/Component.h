// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/IComponent.h>
#include <dxi/core/IOS.h>

namespace dxi
{
	namespace scene
	{
		class Component : public IComponent
		{
		public:
			Component( core::IOS * os, std::string name );
			~Component();

			core::IOS * GetOS();
			const core::IOS * GetOS() const;

			std::string GetName() const override;
			void SetName( std::string name ) override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

		private:
			core::IOS * m_os;
			std::string m_name;
			bool m_enabled;
		};
	}
}