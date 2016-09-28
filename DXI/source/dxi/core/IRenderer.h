// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Viewport.h>

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

			// *** Viewport ***
			
			/// <summary>
			/// Get the number of viewports.
			/// </summary>
			virtual size_t GetNumberOfViewports() const = 0;

			/// <summary>
			/// Get the current viewport, or first viewport if multiple viewports are supported.
			/// </summary>
			virtual void GetViewport( Viewport & viewport ) = 0;

			/// <summary>
			/// Set the current viewport, or first viewport if multiple viewports are supported.
			/// </summary>
			virtual void SetViewport( const Viewport & viewport ) = 0;

			/// <summary>
			/// Get all viewports, upto numberOfViewports (assumes it's the size of viewports argument). Updates
			/// numberOfViewports to the number of viewports we've actually updated, in viewports argument.
			/// </summary>
			virtual void GetViewports( size_t & numberOfViewports, Viewport * viewports ) = 0;

			/// <summary>
			/// Set viewports from an array of viewports.
			/// </summary>
			virtual void SetViewports( size_t & numberOfViewports, const Viewport * viewports ) = 0;

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
		};
	}
}

