// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/shader/ConstantBuffer.h>
#include <me/VertexDeclaration.h>
#include <me/Mercury.h>
#include <rm/IResource.h>
#include <unify/Path.h>
#include <unify/DataLock.h>
#include <vector>
#include <memory>

// TODO: Move to shader namespace.

namespace me 
{
	struct VertexShaderParameters
	{
		VertexShaderParameters()
			: trans{ false }
		{
		}

		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		shader::ConstantBuffer::ptr constants;
		VertexDeclaration::ptr vertexDeclaration;
		bool trans;
	};

	class IVertexShader : public rm::IResource
	{
	public:
		typedef std::shared_ptr< IVertexShader > ptr;		

		virtual ~IVertexShader() {}

		virtual const shader::ConstantBuffer * GetConstants() const = 0;
		virtual void LockConstants( size_t buffer, unify::DataLock & lock ) = 0;
		virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;
		virtual VertexDeclaration::ptr GetVertexDeclaration() const = 0;
		virtual const void * GetBytecode() const = 0;
		virtual size_t GetBytecodeLength() const = 0;
		virtual void Use() = 0;
		virtual unify::Path GetSource() const = 0;
		virtual bool IsTrans() const = 0;
	};
}