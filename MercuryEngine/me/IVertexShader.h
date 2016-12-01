// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/VertexDeclaration.h>
#include <me/RenderInfo.h>
#include <me/RenderInstance.h>
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
		
		VertexShaderParameters(
			unify::Path _path,
			std::string _code,
			std::string _entryPointName,
			std::string _profile,
			VertexDeclaration::ptr _vertexDeclaration
		)
			: path( _path )
			, code( _code )
			, entryPointName( _entryPointName )
			, profile( _profile )
			, vertexDeclaration( _vertexDeclaration )
		{
		}

		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		VertexDeclaration::ptr vertexDeclaration;
	};

	class IVertexShader
	{
	public:
		typedef std::shared_ptr< IVertexShader > ptr;

		virtual ~IVertexShader() {}

		virtual VertexDeclaration::ptr GetVertexDeclaration() const = 0;
		virtual const void * GetBytecode() const = 0;
		virtual size_t GetBytecodeLength() const = 0;
		virtual void Use( const RenderInfo & renderInfo, const unify::Matrix & world ) = 0;
	};
}