// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace me 
{
	namespace render
	{
		class IVertexShader;
		class VertexDeclaration;

		class IVertexConstruct
		{
		public:
			typedef std::shared_ptr< IVertexConstruct > ptr;

			~IVertexConstruct() {}

			virtual void Use() const = 0;
		};
	}
}
