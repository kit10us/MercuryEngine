// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace me 
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
