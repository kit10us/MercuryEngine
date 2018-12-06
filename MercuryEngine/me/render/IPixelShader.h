// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/shader/ConstantBuffer.h>
#include <me/Mercury.h>
#include <me/render/Blend.h>
#include <rm/IResource.h>
#include <unify/Path.h>
#include <unify/DataLock.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct PixelShaderParameters
		{
			PixelShaderParameters()
				: trans{ false }
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

		class IPixelShader : public rm::IResource
		{
		public:
			typedef std::shared_ptr< IPixelShader > ptr;

			virtual ~IPixelShader() {}

			virtual const shader::ConstantBuffer * GetConstants() const = 0;
			virtual void LockConstants( size_t buffer, unify::DataLock & lock ) = 0;
			virtual void UnlockConstants( size_t buffer, unify::DataLock & lock ) = 0;

			virtual void Use() = 0;

			virtual bool IsTrans() const = 0;
		};
	}
}