#pragma once

#include <POD.h>
#include <windows.h>

#ifdef CreateDialog // Remove Microsoft definition.
#undef CreateDialog
#endif

class DisplayDetails
{
    // Private to prevent direct creation...
    DisplayDetails();

public: // Named constructors - these are static functions that return an instance of this class specifically created for a purpose, exposed by a name.
    
    static DisplayDetails CreateDialog( WORD dialogResourceID, HWND parent = HWND(), const V2 position = V2(), DLGPROC dialogProc = 0 );

    static DisplayDetails CreateWindowedDirectXDisplay( const Size size, const V2 position = V2(), HWND parent = HWND() );

    static DisplayDetails CreateFullscreenDirectXDisplay( const Size size );

public:

    // Are we a dialog, created through the Dialog Editor?
    // ...else we a dedicated Direct-X display.
    bool IsDialog() const;

    bool IsFullscreen() const;

    Size GetSize() const;

    V2 GetPosition() const;

    HWND GetParentHandle() const;

	const char * GetDialogTemplateName() const;

	DLGPROC GetDialogProc() const;

private:
    // Various...
    bool m_isDialog;
    HWND m_parentHandle;
    Size m_size;
    V2 m_position;

    // Dialog only...
    WORD m_resourceID;
    std::string m_resourceName;
	DLGPROC m_dialogProc;

    // Dedicated Direct-X only...
    bool m_isFullscreen;
};