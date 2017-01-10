// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/shader/ShaderConstants.h>
#include <me/Mercury.h>
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
			shader::ShaderConstants::ptr _constants
		)
			: path( _path )
			, code( _code )
			, entryPointName( _entryPointName )
			, profile( _profile )
			, constants( _constants )
		{
		}


		unify::Path path;
		std::string code;
		std::string entryPointName;
		std::string profile;
		shader::ShaderConstants::ptr constants;
	};

	class IPixelShader
	{
	public:
		typedef std::shared_ptr< IPixelShader > ptr;

		virtual ~IPixelShader() {}

		virtual void SetTrans( bool bTrans ) = 0;

		virtual bool IsTrans() = 0;

		virtual const shader::ShaderConstants * GetConstants() const = 0;
		virtual void LockConstants( size_t buffer, unify::DataLock & lock ) = 0;
		virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;

		virtual void Use() = 0;
	};
}