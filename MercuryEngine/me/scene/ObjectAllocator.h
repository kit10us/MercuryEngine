// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/scene/IObjectAllocator.h>

namespace me
{
	namespace scene
	{
		class ObjectAllocator : public IObjectAllocator
		{
		public:
			ObjectAllocator( std::string typeName );			

		public: // IUnknown...
			std::string GetTypeName() const override;
			std::string GetWhat() const override;
			IUnknown* QueryInterface( std::string name ) override;

		protected:
			void AddInterface( std::string name, IUnknown* ptr );

		private:
			std::map< std::string, IUnknown* > m_interfaces;
			std::string m_typeName;
		};
	}
}