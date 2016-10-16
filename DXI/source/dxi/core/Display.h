// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <Windows.h>
#include <dxi/Viewport.h>
#include <unify/Size.h>
#include <unify/V2.h>
#include <unify/MinMax.h>

#ifdef CreateDialog
#undef CreateDialog
#endif

namespace dxi
{
	namespace core
	{
		class Display
		{
		public:
			Display();
			static Display CreateDialog( WORD dialogResourceID, HWND parent = HWND(), unify::V2< float > position = unify::V2< float >( 0.0f, 0.0f ), DLGPROC dialogProc = 0 );
			static Display CreateWindowedDirectXDisplay( unify::Size< float > size, unify::V2< float > position = unify::V2< float >( 0.0f, 0.0f ), HWND handle = {}, HWND parent = {} );
			static Display CreateFullscreenDirectXDisplay( unify::Size< float > size, HWND handle = {} );

			// Are we a dialog, created through the Dialog Editor?
			// ...else we a dedicated Direct-X display.
			bool IsDialog() const;

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

			const char * GetDialogTemplateName() const;

			DLGPROC GetDialogProc() const;

		private:
			// Various...
			bool m_isDialog;
			HWND m_handle;
			HWND m_parentHandle;
			unify::Size< float > m_size;
			unify::V2< float > m_position;
			float m_nearZ;
			float m_farZ;

			// Dialog only...
			WORD m_resourceID;
			std::string m_resourceName;
			DLGPROC m_dialogProc;

			// Dedicated Direct-X only...
			bool m_isFullscreen;
		};
	}
}