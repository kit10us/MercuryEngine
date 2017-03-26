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
		{
		}

		PixelShaderParameters(
			unify::Path _path,
			std::string _code,
			std::string _entryPointName,
			std::string _profile,
			shader::ConstantBuffer::ptr _constants,
			BlendDesc _blend
		)
			: path( _path )
			, code( _code )
			, entryPointName( _entryPointName )
			, profile( _profile )
			, constants( _constants )
			, blendDesc( _blend )
		{
		}

		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		shader::ConstantBuffer::ptr constants;
		BlendDesc blendDesc;
	};

	class IPixelShader
	{
	public:
		typedef std::shared_ptr< IPixelShader > ptr;

		virtual ~IPixelShader() {}

		virtual void SetTrans( bool bTrans ) = 0;

		virtual bool IsTrans() = 0;

		virtual const shader::ConstantBuffer * GetConstants() const = 0;
		virtual void LockConstants( size_t buffer, unify::DataLock & lock ) = 0;
		virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;

		virtual void Use() = 0;

		virtual std::string GetSource() const = 0;

		virtual bool Reload() = 0;
	};
}