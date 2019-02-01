// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/Viewport.h>
#include <unify/Size.h>
#include <unify/V2.h>

namespace me
{
	namespace render
	{
		class Display
		{
		public:
			Display();
			static Display CreateWindowedDirectXDisplay( unify::Size< float > size, unify::V2< float > position = unify::V2< float >( 0.0f, 0.0f ), void* handle = 0, void* parent = 0 );
			static Display CreateFullscreenDirectXDisplay( unify::Size< float > size, void* handle = 0 );

			bool IsFullscreen() const;

			unify::Size< float > GetSize() const;

			unify::V2< float > GetPosition() const;

			float GetNearZ() const;
			float GetFarZ() const;

			void SetNearZ( float z );
			void SetFarZ( float z );

			void* GetHandle() const;
			void SetHandle( void* handle );

			void* GetParentHandle() const;

		private:
			void* m_handle;
			void* m_parentHandle;
			unify::Size< float > m_size;
			unify::V2< float > m_position;
			float m_nearZ;
			float m_farZ;
			bool m_isFullscreen;
		};
	}
}