// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <windows.h> // TODO: Because of handle - fix?
#include <dxi/Viewport.h>
#include <dxi/RenderInfo.h>

namespace dxi
{
	struct CullMode
	{
		enum TYPE
		{
			None,
			Clockwise,
			CounteClockwise
		};
	};

	namespace core
	{
		/// <summary>
		/// Supports access to the renderer.
		/// </summary>
		class IRenderer
		{
		public:
			virtual ~IRenderer() {}

			virtual Viewport GetViewport() const = 0;

			/// <summary>
			/// Called before render to allow renderer to perapre.
			/// </summary>
			virtual void BeforeRender() = 0;

			/// <summary>
			/// Called after render to allow renderer to cleanup.
			/// </summary>
			virtual void AfterRender() = 0;

			// New support for render states
			virtual void SetCullMode( CullMode::TYPE mode ) = 0;

			virtual bool IsFullscreen() const = 0;

			virtual HWND GetHandle() const = 0;
		};
	}
}

