// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/TextureMode.h>
#include <me/render/TextureLock.h>
#include <me/render/Filtering.h>
#include <me/render/SpriteDictionary.h>
#include <me/render/Format.h>
#include <rm/IResource.h>
#include <qxml/Element.h>
#include <unify/DataLock.h>
#include <unify/Path.h>
#include <unify/Size.h>
#include <unify/Rect.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct ShaderResourceParameters
		{
			ShaderResourceParameters()
				: usage{}
				, type{}
			{
			}

			BufferUsage::TYPE usage;
			ResourceType::TYPE type;
		};

		class IShaderResource : public rm::IResource
		{
		public:
			typedef std::shared_ptr< IShaderResource > ptr;

			virtual ~IShaderResource() {}

			/// <summary>
			/// Use the shader resource.
			/// </summary>
			virtual void Use() const = 0;

			/// <summary>
			/// Lock resource.
			/// </summary>
			virtual void Lock( unsigned int level, unify::DataLock & lock, unify::DataLockAccess::TYPE type ) = 0;

			/// <summary>
			/// Unlock resource.
			/// </summary>
			virtual void Unlock( unsigned int level ) = 0;
		};
	}
}