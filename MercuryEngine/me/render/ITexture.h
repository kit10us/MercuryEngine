// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/TextureMode.h>
#include <me/render/TextureLock.h>
#include <me/render/Filtering.h>
#include <me/render/SpriteDictionary.h>
#include <me/render/BufferUsage.h>
#include <me/render/Format.h>
#include <qxml/Element.h>
#include <rm/IResource.h>
#include <unify/Path.h>
#include <unify/Size.h>
#include <unify/Rect.h>
#include <unify/DataLock.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct TextureParameters
		{
			TextureParameters();
			TextureParameters( unify::Path _source, bool _renderable = true, bool _lockable = false );
			TextureParameters( const qxml::Element * element );

			unify::Path source;
			
			unify::Size< unsigned int > size; // Ignored if we have a source path.

			BufferUsage::TYPE usage;
			Format::TYPE format;			
			
			bool renderable;
			bool lockable;

			Filtering::TYPE min;
			Filtering::TYPE mag;
			Filtering::TYPE mip;
		};

		class ITexture : public rm::IResource
		{
		public:
			typedef std::shared_ptr< ITexture > ptr;

			virtual ~ITexture() {}

			/// <summary>
			/// Get the size of the texture's image.
			/// </summary>
			virtual const unify::Size< unsigned int > & ImageSize() const = 0;

			/// <summary>
			/// Returns true if the texture can be used to rendered.
			/// </summary>
			virtual bool GetRenderable() const = 0;

			/// <summary>
			/// Returns true if the texture can be used to rendered.
			/// </summary>
			virtual bool GetLockable() const = 0;

			/// <summary>
			/// Lock a rectangle.
			/// </summary>
			virtual void LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, unify::DataLockAccess::TYPE access ) = 0;

			/// <summary>
			/// Unlock a rectangle.
			/// </summary>
			virtual void UnlockRect( unsigned int level ) = 0;

			/// <summary>
			/// Get the Sprite Dictionary, used to track image fragments/sprites/fonts.
			/// <summary>
			virtual SpriteDictionary & GetSpriteDictionary() = 0;

			/// <summary>
			/// Get the Sprite Dictionary, used to track image fragments/sprites/fonts.
			/// <summary>
			virtual const SpriteDictionary & GetSpriteDictionary() const = 0;

			/// <summary>
			/// Get the parameters used for creation.
			/// </summary>
			virtual const TextureParameters * GetParameters() const = 0;
		};
	}
}