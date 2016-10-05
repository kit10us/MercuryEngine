#include <TesterBase.h>
#include <resource.h>
#include <assert.h>
#include <list>
#include <boost/lexical_cast.hpp>
#include <map>
#include <boost/algorithm/string.hpp>

template< typename T >
std::string ToString( T format, const T * values, const std::string * strings )
{
    for( int i = 0; ! strings[ i ].empty(); ++i )
	{
		if ( values[ i ] == format )
		{
			return strings[ i ];
		}
	}
	return "--ERROR--";
}

std::string FlagsToString( unsigned int flags, const std::map< std::string, unsigned int > & nameToFlag )
{
    typedef std::map< std::string, unsigned int > MapType;
    std::string output;
    for ( MapType::const_iterator itr = nameToFlag.begin(), end = nameToFlag.end(); itr != end; ++itr )
    {
        if ( flags & itr->second )
        {
            if ( ! output.empty() )
            {
                output += " | ";
            }
            output += itr->first;
        }
    }    
    return output;
}



// D3DFORMAT from #include<d3d9types.h>
D3DFORMAT g_D3DFORMATValue[] =
{
	D3DFMT_UNKNOWN,

	D3DFMT_R8G8B8,
	D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,
	D3DFMT_R5G6B5,
	D3DFMT_X1R5G5B5,
	D3DFMT_A1R5G5B5,
	D3DFMT_A4R4G4B4,
	D3DFMT_R3G3B2,
	D3DFMT_A8,
	D3DFMT_A8R3G3B2,
	D3DFMT_X4R4G4B4,
	D3DFMT_A2B10G10R10,
	D3DFMT_A8B8G8R8,
	D3DFMT_X8B8G8R8,
	D3DFMT_G16R16,
	D3DFMT_A2R10G10B10,
	D3DFMT_A16B16G16R16,

	D3DFMT_A8P8,
	D3DFMT_P8,

	D3DFMT_L8,
	D3DFMT_A8L8,
	D3DFMT_A4L4,

	D3DFMT_V8U8,
	D3DFMT_L6V5U5,
	D3DFMT_X8L8V8U8,
	D3DFMT_Q8W8V8U8,
	D3DFMT_V16U16,
	D3DFMT_A2W10V10U10,

	D3DFMT_UYVY,
	D3DFMT_R8G8_B8G8,
	D3DFMT_YUY2,
	D3DFMT_G8R8_G8B8,
	D3DFMT_DXT1,
	D3DFMT_DXT2,
	D3DFMT_DXT3,
	D3DFMT_DXT4,
	D3DFMT_DXT5,

	D3DFMT_D16_LOCKABLE,
	D3DFMT_D32,
	D3DFMT_D15S1,
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D24X4S4,
	D3DFMT_D16,

	D3DFMT_D32F_LOCKABLE,
	D3DFMT_D24FS8,

	D3DFMT_L16,

	D3DFMT_VERTEXDATA,
	D3DFMT_INDEX16,
	D3DFMT_INDEX32,

	D3DFMT_Q16W16V16U16,

	D3DFMT_MULTI2_ARGB8,

	D3DFMT_R16F,
	D3DFMT_G16R16F,
	D3DFMT_A16B16G16R16F,

	D3DFMT_R32F,
	D3DFMT_G32R32F,
	D3DFMT_A32B32G32R32F,

	D3DFMT_CxV8U8
};

std::string g_D3DFORMATString[] =
{
	"UNKNOWN",

	"R8G8B8",
	"A8R8G8B8",
	"X8R8G8B8",
	"R5G6B5",
	"X1R5G5B5",
	"A1R5G5B5",
	"A4R4G4B4",
	"R3G3B2",
	"A8",
	"A8R3G3B2",
	"X4R4G4B4",
	"A2B10G10R10",
	"A8B8G8R8",
	"X8B8G8R8",
	"G16R16",
	"A2R10G10B10",
	"A16B16G16R16",

	"A8P8",
	"P8",

	"L8",
	"A8L8",
	"A4L4",

	"V8U8",
	"L6V5U5",
	"X8L8V8U8",
	"Q8W8V8U8",
	"V16U16",
	"A2W10V10U10",

	"UYVY",
	"R8G8_B8G8",
	"YUY2",
	"G8R8_G8B8",
	"DXT1",
	"DXT2",
	"DXT3",
	"DXT4",
	"DXT5",

	"D16_LOCKABLE",
	"D32",
	"D15S1",
	"D24S8",
	"D24X8",
	"D24X4S4",
	"D16",

	"D32F_LOCKABLE",
	"D24FS8",

	"L16",

	"VERTEXDATA",
	"INDEX16",
	"INDEX32",

	"Q16W16V16U16",

	"MULTI2_ARGB8",

	"R16F",
	"G16R16F",
	"A16B16G16R16F",

	"R32F",
	"G32R32F",
	"A32B32G32R32F",

	"CxV8U8",

	""
};

std::string ToString( D3DFORMAT format )
{
    return ToString( format, g_D3DFORMATValue, g_D3DFORMATString );
}

DWORD g_FVFValue[] =
{
    //D3DFVF_RESERVED0,
    //D3DFVF_POSITION_MASK,
    D3DFVF_XYZ,
    D3DFVF_XYZRHW,
    D3DFVF_XYZB1,
    D3DFVF_XYZB2,
    D3DFVF_XYZB3,
    D3DFVF_XYZB4,
    D3DFVF_XYZB5,
    D3DFVF_XYZW,

    D3DFVF_NORMAL,
    D3DFVF_PSIZE,
    D3DFVF_DIFFUSE,
    D3DFVF_SPECULAR,

    //D3DFVF_TEXCOUNT_MASK,
    //D3DFVF_TEXCOUNT_SHIFT,
    D3DFVF_TEX0,
    D3DFVF_TEX1,
    D3DFVF_TEX2,
    D3DFVF_TEX3,
    D3DFVF_TEX4,
    D3DFVF_TEX5,
    D3DFVF_TEX6,
    D3DFVF_TEX7,
    D3DFVF_TEX8,

    D3DFVF_LASTBETA_UBYTE4,
    D3DFVF_LASTBETA_D3DCOLOR,

    //D3DFVF_RESERVED2
};

std::string g_FVFString[] =
{
    //"RESERVED0",
    //"POSITION_MASK",
    "XYZ",
    "XYZRHW",
    "XYZB1",
    "XYZB2",
    "XYZB3",
    "XYZB4",
    "XYZB5",
    "XYZW",

    "NORMAL",
    "PSIZE",
    "DIFFUSE",
    "SPECULAR",

    //"TEXCOUNT_MASK",
    //"TEXCOUNT_SHIFT",
    "TEX0",
    "TEX1",
    "TEX2",
    "TEX3",
    "TEX4",
    "TEX5",
    "TEX6",
    "TEX7",
    "TEX8",

    "LASTBETA_UBYTE4",
    "LASTBETA_D3DCOLOR",

    //"RESERVED2"
    ""
};

std::string FVFToString( DWORD format )
{
    std::string out;
    for( int i = 0; ! g_FVFString[ i ].empty(); ++i )
	{
		if ( g_FVFValue[ i ] | format )
		{
            if ( ! out.empty() )
            {
                out += " | ";
            }
            out += g_FVFString[ i ];
		}
	}
	return out;
}


class Tester : public TesterBase
{
public:
	void BeforeStartup( const std::string & commandLine );
	virtual LRESULT HandleWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	void Startup();
	void ResetTextureSettings();
	void RecreateTexture();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 & dxDevice );
	void Shutdown();

protected:
	void PopulateAdapterList();
	void OnAdapterSelect();

	// I prefer to gather my handles and keep them, as I believe it keeps the implementation cleaner.
	HWND m_hwndAdapterList;
	HWND m_hwndAdapterDetails;
	HWND m_hwndList1;
} tester;


void Tester::BeforeStartup( const std::string & commandLine )
{
	commandLine; // Not used

	CreatePrimaryDisplay( DisplayDetails::CreateDialog( IDD_ENUM ) );
}

LRESULT Tester::HandleWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    hWnd; // NOT USED
    unsigned short wHigh = HIWORD( wParam );
    wHigh; // NOT USED
    unsigned short wLow = LOWORD( wParam );
    lParam; // NOT USED

	switch( message )
	{
	case WM_INITDIALOG:
		{
			// Gather dialog item handles...
			m_hwndAdapterList = GetDlgItem( hWnd, IDC_ADAPTERLIST );
			m_hwndAdapterDetails = GetDlgItem( hWnd, IDC_ADAPTERDETAILS );
			m_hwndList1 = GetDlgItem( hWnd, IDC_LIST1 );

			PopulateAdapterList();
		}
		break;

	case WM_COMMAND:
		switch( wLow )
		{
		case IDOK:
			{
				int x(0);x;
			}
			break;

		case IDCANCEL:
			RequestQuit();
			break;

		case IDC_ADAPTERLIST:
			{
				switch ( wHigh )
				{
				case LBN_SELCHANGE:
					{
						OnAdapterSelect();
					}
				}
			}
			break;

		case IDC_LIST1:
			break;

		}
		break;
	default: 
		break;
	}

	return TesterBase::HandleWndProc( hWnd, message, wParam, lParam );
}

void Tester::PopulateAdapterList()
{
	SendMessageA( m_hwndAdapterList, LB_RESETCONTENT, 0, 0 );

	IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
	unsigned int adapterCount = dx->GetAdapterCount();
	for( unsigned int adapter = 0; adapter != adapterCount; ++adapter )
	{
		D3DADAPTER_IDENTIFIER9 identifier;
		dx->GetAdapterIdentifier( adapter, 0, &identifier );
		std::string deviceName = std::string( identifier.DeviceName ) + " (" + identifier.Description + ")";
		int position = (int)SendMessageA( m_hwndAdapterList, LB_ADDSTRING, 0, (LPARAM)deviceName.c_str() );
		SendMessageA( m_hwndAdapterList, LB_SETITEMDATA, position, (LPARAM)adapter );
	}

	SetWindowTextA( m_hwndAdapterDetails, "Select an adapter..." );

	SendMessageA( m_hwndList1, LB_RESETCONTENT, 0, 0 );
	SendMessageA( m_hwndList1, LB_ADDSTRING, 0, (LPARAM)"Select an adapter..." );
	EnableWindow( m_hwndList1, false );
	dx->Release();
}

void Tester::OnAdapterSelect()
{
	std::string detailsText;

	IDirect3D9 * dxInterface = Direct3DCreate9( D3D_SDK_VERSION );

    HRESULT hr = S_OK;

	int selectedAdapterInList = (int)SendMessageA( m_hwndAdapterList, LB_GETCURSEL, 0, 0 );
	int adapter = (int)SendMessageA( m_hwndAdapterList, LB_GETITEMDATA, selectedAdapterInList, 0 );
	D3DADAPTER_IDENTIFIER9 identifier;
	hr = dxInterface->GetAdapterIdentifier( adapter, 0, &identifier );
    assert( SUCCEEDED( hr ) );
	
	detailsText += std::string( "name:  " ) + identifier.DeviceName + "\r\n";
	detailsText += std::string( "description:  " ) + identifier.Description + "\r\n";
	detailsText += std::string( "driver:  " ) + identifier.Driver + "\r\n";
    detailsText += std::string( "\r\n" );
    
    detailsText += std::string( "HAL Capabilities:  " ) + "\r\n";
    D3DDEVTYPE typeEnum = D3DDEVTYPE_HAL;

    {
        D3DCAPS9 caps;
        hr = dxInterface->GetDeviceCaps( adapter, typeEnum, &caps );
        assert( SUCCEEDED( hr ) );

        detailsText += std::string( "NumberOfAdaptersInGroup = " ) + boost::lexical_cast< std::string, unsigned int >( caps.NumberOfAdaptersInGroup ) + "\r\n";
        
        detailsText += std::string( "FVFCaps = " ) + FVFToString( caps.FVFCaps );
        
        std::map< std::string, unsigned int > presentationIntervals;
        presentationIntervals[ "D3DPRESENT_INTERVAL_IMMEDIATE" ] = D3DPRESENT_INTERVAL_IMMEDIATE;
        presentationIntervals[ "D3DPRESENT_INTERVAL_ONE" ] = D3DPRESENT_INTERVAL_ONE;
        presentationIntervals[ "D3DPRESENT_INTERVAL_TWO" ] = D3DPRESENT_INTERVAL_TWO;
        presentationIntervals[ "D3DPRESENT_INTERVAL_THREE" ] = D3DPRESENT_INTERVAL_THREE;
        presentationIntervals[ "D3DPRESENT_INTERVAL_FOUR" ] = D3DPRESENT_INTERVAL_FOUR;
        detailsText += std::string( "PresentationIntervals = " ) + FlagsToString( caps.PresentationIntervals, presentationIntervals ) + "\r\n";

        detailsText += "\r\n";
    }

    // Current display mode for said adapter.
	D3DDISPLAYMODE displayMode;
	dxInterface->GetAdapterDisplayMode( adapter, &displayMode );
	{
		std::string widthAsString = boost::lexical_cast< std::string >( displayMode.Width );
		std::string heightAsString = boost::lexical_cast< std::string >( displayMode.Height );
		std::string formatAsString = dx::Format::ToString( displayMode.Format );
		std::string refreshRateAsString = boost::lexical_cast< std::string >( displayMode.RefreshRate );
		
		detailsText += std::string( "mode: " ) + widthAsString + "x" + heightAsString + " - " + formatAsString + "@" + refreshRateAsString + "hz" + "\r\n" + "\r\n";
	}

	SetWindowTextA( m_hwndAdapterDetails, detailsText.c_str() );

	// Available display modes...
	SendMessageA( m_hwndList1, LB_RESETCONTENT, 0, 0 );
	unsigned int modes = unsigned int();
	for ( int i = 0; ! g_D3DFORMATString[ i ].empty(); ++i )
	{
		D3DFORMAT format = g_D3DFORMATValue[ i ];
		unsigned int modeCount = dxInterface->GetAdapterModeCount( adapter, format );
		for ( unsigned int mode = 0; mode != modeCount; ++ mode )
		{
			D3DDISPLAYMODE displayMode;
			dxInterface->EnumAdapterModes( adapter, format, mode, &displayMode );
			std::string widthAsString = boost::lexical_cast< std::string >( displayMode.Width );
			std::string heightAsString = boost::lexical_cast< std::string >( displayMode.Height );
			std::string formatAsString = ToString( displayMode.Format );
			std::string refreshRateAsString = boost::lexical_cast< std::string >( displayMode.RefreshRate );
			std::string modeText( widthAsString + "x" + heightAsString + " - " + formatAsString + " @" + refreshRateAsString );
			SendMessageA( m_hwndList1, LB_ADDSTRING, 0, (LPARAM)modeText.c_str() );
			++modes;
		}
	}
	EnableWindow( m_hwndList1, modes > 0 ? true : false );
	int x(0);x;
}

void Tester::Startup()
{
}

void Tester::Update( float delta, Input & input )
{
    delta; input;
}

void Tester::Render( IDirect3DDevice9 & dxDevice )
{
	dxDevice; // Not used
}

void Tester::Shutdown()
{
}
