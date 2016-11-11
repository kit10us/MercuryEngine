// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/TextureMode.h>
#include <me/TextureLock.h>
#include <me/Filtering.h>
#include <unify/Path.h>
#include <unify/Size.h>
#include <unify/Rect.h>
#include <memory>

namespace me 
{
	struct TextureParameters
	{
		TextureParameters()
			: renderable( true )
			, lockable( false )
			, min( me::Filtering::Linear )
			, mag( me::Filtering::Linear )
			, mip( me::Filtering::Linear )
		{
		}

		TextureParameters( unify::Path _source, bool _renderable = true, bool _lockable = false )
			: renderable( _renderable )
			, lockable( _lockable )
			, min( me::Filtering::Linear )
			, mag( me::Filtering::Linear )
			, mip( me::Filtering::Linear )
		{
		}

		unify::Path source;
		bool renderable;
		bool lockable;
		me::Filtering::TYPE min;
		me::Filtering::TYPE mag;
		me::Filtering::TYPE mip;
	};


	class ITexture
	{
	public:
		typedef std::shared_ptr< ITexture > ptr;

		virtual ~ITexture() {}

		virtual const unify::Size< unsigned int > & ImageSize() const = 0;

		virtual bool Use( unsigned int stage ) = 0;

		virtual bool GetRenderable() const = 0;

		virtual bool GetLockable() const = 0;

		virtual void LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly ) = 0;
		
		virtual void UnlockRect( unsigned int level ) = 0;
	};
}