// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
		struct TextureLockAccess
		{
			unify::DataLockAccess ::TYPE cpu;
			unify::DataLockAccess::TYPE gpu;
		};

		struct TextureParameters
		{
			TextureParameters();

			/// <summary>
			/// Set the texture parameters for texture creation. Setting both the cpu and gpu to None lets the texture header decide, else these values override the texture header.
			TextureParameters( unify::Path _source, TextureLockAccess lockAccess = { unify::DataLockAccess::None, unify::DataLockAccess::None } );
			TextureParameters( const qxml::Element * element );

			unify::Path source;
			
			unify::Size< unsigned int > size; // Ignored if we have a source path.

			BufferUsage::TYPE usage;
			Format::TYPE format;			

			TextureLockAccess lockAccess;

			Filtering::TYPE min;
			Filtering::TYPE mag;
			Filtering::TYPE mip;
		};
	}
}