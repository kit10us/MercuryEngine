#include <DisplayDetails.h>
#include <assert.h>

DisplayDetails::DisplayDetails()
// No initialization list, because the data can be so different based on the used named constructors.
// It would also be inefficient.
{
}

DisplayDetails DisplayDetails::CreateDialog( WORD dialogResourceID, HWND parent, const V2 position, DLGPROC dialogProc )
{
    DisplayDetails details;
    details.m_isDialog = true;
    details.m_parentHandle = parent;
    details.m_position = position;
    details.m_size = Size();
    details.m_resourceID = dialogResourceID;
    details.m_resourceName.clear(); // What ever - ignored later.
    details.m_isFullscreen = false; // What ever - ignored later.
	details.m_dialogProc = dialogProc;
    return details;
}

DisplayDetails DisplayDetails::CreateWindowedDirectXDisplay( const Size size, const V2 position, HWND parent )
{
    DisplayDetails details;
    details.m_isDialog = false;
    details.m_parentHandle = parent;
    details.m_position = position;
    details.m_size = size;
    details.m_resourceID = 0; // What ever - ignored later.
    details.m_resourceName.clear(); // What ever - ignored later.
    details.m_isFullscreen = false;
	details.m_dialogProc = 0;
    return details;
}

DisplayDetails DisplayDetails::CreateFullscreenDirectXDisplay( const Size size )
{
    DisplayDetails details;
    details.m_isDialog = false;
    details.m_parentHandle = 0;
    details.m_position = V2(); // What ever - ignored later.
    details.m_size = size;
    details.m_resourceID = 0; // What ever - ignored later.
    details.m_resourceName.clear(); // What ever - ignored later.
    details.m_isFullscreen = true;
	details.m_dialogProc = 0;
    return details;
}

bool DisplayDetails::IsDialog() const
{
    return m_isDialog;
}

bool DisplayDetails::IsFullscreen() const
{
    return m_isFullscreen;
}

Size DisplayDetails::GetSize() const
{
    return m_size;
}

V2 DisplayDetails::GetPosition() const
{
    return m_position;
}

HWND DisplayDetails::GetParentHandle() const
{
    return m_parentHandle;
}

const char * DisplayDetails::GetDialogTemplateName() const
{
	return ! m_resourceName.empty() ? m_resourceName.c_str() : MAKEINTRESOURCEA( m_resourceID );
}

DLGPROC DisplayDetails::GetDialogProc() const
{
	return m_dialogProc;
}