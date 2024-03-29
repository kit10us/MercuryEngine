// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/IConstantBuffer.h>
#include <me/render/VertexDeclaration.h>
#include <me/Mercury.h>
#include <rm/IResource.h>
#include <unify/Path.h>
#include <memory>

namespace me 
{
	namespace render
	{
		class IShader : public rm::IResource
		{
		public:
			typedef std::shared_ptr< IShader > ptr;

			virtual ~IShader() {}

			/// <summary>
			/// Create a constant buffer.
			/// </summary>
			virtual IConstantBuffer::ptr CreateConstantBuffer( BufferUsage::TYPE usage ) const = 0;

			virtual const void * GetBytecode() const = 0;
			virtual size_t GetBytecodeLength() const = 0;
			virtual void Use() = 0;
			virtual bool IsTrans() const = 0;
		};
	}
}