// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/Viewport.h>
#include <me/Display.h>
#include <me/IVertexBuffer.h>
#include <me/IIndexBuffer.h>
#include <me/IVertexShader.h>
#include <me/IPixelShader.h>
#include <me/ITexture.h>
#include <me/RenderInfo.h>
#include <me/RenderMethod.h>
#include <unify/FrameLite.h>
#include <memory>

#include <me/MatrixFeed.h>

namespace me
{
	/// <summary>
	/// Allows drawing simply shapes.
	/// </summary>
	class ISketcher
	{
	public:
		virtual ~ISketcher() {}
	};
}

