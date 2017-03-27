// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/shader/ConstantBuffer.h>
#include <me/Mercury.h>
#include <me/Blend.h>
#include <unify/Path.h>
#include <unify/DataLock.h>
#include <memory>

namespace me 
{
	struct PixelShaderParameters
	{
		PixelShaderParameters()
			:  trans{ false }
		{
		}

		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		shader::ConstantBuffer::ptr constants;
		BlendDesc blendDesc;
		bool trans;
	};

	class IPixelShader
	{
	public:
		typedef std::shared_ptr< IPixelShader > ptr;

		virtual ~IPixelShader() {}

		virtual const shader::ConstantBuffer * GetConstants() const = 0;
		virtual void LockConstants( size_t buffer, unify::DataLock & lock ) = 0;
		virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;

		virtual void Use() = 0;

		virtual std::string GetSource() const = 0;

		virtual bool Reload() = 0;

		virtual bool IsTrans() const = 0;
	};
}