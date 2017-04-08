// Copyright (c) 2002 - 2018, Quentin S. Smith
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

		public: // me::IThing...
			std::string GetTypeName() const override;
			std::string GetWhat() const override;
			me::IThing* QueryInterface( std::string name ) override;

		protected:
			void AddInterface( std::string name, me::IThing* ptr );

		private:
			std::map< std::string, me::IThing* > m_interfaces;
			std::string m_typeName;
		};
	}
}