// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IVertexDeclaration.h>
#include <me/RenderInfo.h>
#include <me/Mercury.h>
#include <unify/Path.h>
#include <memory>

namespace me 
{
	struct VertexShaderParameters
	{
		VertexShaderParameters()
		{
		}
		
		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		IVertexDeclaration::ptr vertexDeclaration;
	};

	class IVertexShader
	{
	public:
		typedef std::shared_ptr< IVertexShader > ptr;

		virtual ~IVertexShader() {}

		virtual IVertexDeclaration::ptr GetVertexDeclaration() const = 0;
		virtual const void * GetBytecode() const = 0;
		virtual size_t GetBytecodeLength() const = 0;
		virtual void Use( const RenderInfo & renderInfo ) = 0;
	};
}