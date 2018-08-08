// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/TextureMode.h>
#include <me/render/TextureLock.h>
#include <me/render/Filtering.h>
#include <me/render/SpriteDictionary.h>
#include <rm/IResource.h>
#include <qxml/Element.h>
#include <unify/Path.h>
#include <unify/Size.h>
#include <unify/Rect.h>
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

			virtual const unify::Size< unsigned int > & ImageSize() const = 0;

			virtual bool GetRenderable() const = 0;

			virtual bool GetLockable() const = 0;

			virtual void LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly ) = 0;

			virtual void UnlockRect( unsigned int level ) = 0;

			virtual SpriteDictionary & GetSpriteDictionary() = 0;

			virtual const SpriteDictionary & GetSpriteDictionary() const = 0;

			virtual const TextureParameters * GetParameters() const = 0;
		};
	}
}