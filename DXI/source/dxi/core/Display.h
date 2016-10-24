// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <Windows.h>
#include <dxi/Viewport.h>
#include <unify/Size.h>
#include <unify/V2.h>

namespace dxi
{
	namespace core
	{
		class Display
		{
		public:
			Display();
			static Display CreateWindowedDirectXDisplay( unify::Size< float > size, unify::V2< float > position = unify::V2< float >( 0.0f, 0.0f ), HWND handle = {}, HWND parent = {} );
			static Display CreateFullscreenDirectXDisplay( unify::Size< float > size, HWND handle = {} );

			bool IsFullscreen() const;

			unify::Size< float > GetSize() const;

			unify::V2< float > GetPosition() const;

			float GetNearZ() const;
			float GetFarZ() const;

			void SetNearZ( float z );
			void SetFarZ( float z );

			HWND GetHandle() const;
			void SetHandle( HWND handle );

			HWND GetParentHandle() const;

		private:
			HWND m_handle;
			HWND m_parentHandle;
			unify::Size< float > m_size;
			unify::V2< float > m_position;
			float m_nearZ;
			float m_farZ;
			bool m_isFullscreen;
		};
	}
}