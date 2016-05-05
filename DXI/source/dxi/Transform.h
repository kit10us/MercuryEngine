// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>
#include <unify/unify.h>
#include <unify/Matrix.h>
#include <dxi/exception/OutOfBounds.h>

namespace dxi
{
	struct Transform
	{
		struct Index
		{
			typedef enum 
			{
				View,
				Projection,
				Texture0,
				Texture1,
				Texture2,
				Texture3,
				Texture4,
				Texture5,
				Texture6,
				Texture7,
				World,
				World1,
				World2,
				World3,
				ViewProjection,
				WorldViewProjection,
				ViewProjectionTranspose,
				WorldViewProjectionTranspose,
				COUNT,
				INVALID = COUNT
			} TYPE;
		};

		/*
		// TODO: Hide?
		// Implementation specific address.
		struct Address
		{
			typedef enum 
			{
				View			= D3DTS_VIEW,
				Projection		= D3DTS_PROJECTION,
				Texture0		= D3DTS_TEXTURE0,
				Texture1		= D3DTS_TEXTURE1,
				Texture2		= D3DTS_TEXTURE2,
				Texture3		= D3DTS_TEXTURE3,
				Texture4		= D3DTS_TEXTURE4,
				Texture5		= D3DTS_TEXTURE5,
				Texture6		= D3DTS_TEXTURE6,
				Texture7		= D3DTS_TEXTURE7,
				World			= D3DTS_WORLD,
				World1			= D3DTS_WORLD1,
				World2			= D3DTS_WORLD2,
				World3			= D3DTS_WORLD3,
				ViewProjection	= (512 + 0),
				WorldViewProjection = (512 + 1),
				ViewProjectionTranspose = (512 + 2),
				WorldViewProjectionTranspose = (512 + 3),
				COUNT = 18
			} TYPE;
		};

		static Address::TYPE FromIndex( Index::TYPE index );
		static Index::TYPE FromAddress( Address::TYPE trans );

		static void Set( Index::TYPE index, const unify::Matrix & matrix );
		static void Get( Index::TYPE index, unify::Matrix & matrix );
		*/
	};
} // namespace dxi