// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <memory>

namespace me 
{
	namespace render
	{
		class IVertexConstruct
		{
		public:
			typedef std::shared_ptr< IVertexConstruct > ptr;

			~IVertexConstruct() {}

			virtual void Use() const = 0;
		};
	}
}
