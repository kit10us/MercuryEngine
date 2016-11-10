// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

namespace me
{
	struct TextureLock
	{
		TextureLock()
		{
			pBits = 0;
		}

		unsigned char* pBits; // TODO: rename to data
		unsigned int uStride;		// pos_x * width_stride + pos_y * height_stride == color at pos_x, pos_y // AKA pitch
		unsigned int uWidth;
		unsigned int uHeight;
		size_t bpp;
		size_t totalBytes;
	};
}
