#include <dx/Format.h>

using namespace dx;


Format::TYPE g_Values[] =
{
    Format::Unknown,

    Format::R8G8B8,
    Format::A8R8G8B8,
    Format::X8R8G8B8,
    Format::R5G6B5,
    Format::X1R5G5B5,
    Format::A1R5G5B5,
    Format::A4R4G4B4,
    Format::R3G3B2,
    Format::A8,
    Format::A8R3G3B2,
    Format::X4R4G4B4,
    Format::A2B10G10R10,
    Format::A8B8G8R8,
    Format::X8B8G8R8,
    Format::G16R16,
    Format::A2R10G10B10,
    Format::A16B16G16R16,

    Format::A8P8,
    Format::P8,

    Format::L8,
    Format::A8L8,
    Format::A4L4,

    Format::V8U8,
    Format::L6V5U5,
    Format::X8L8V8U8,
    Format::Q8W8V8U8,
    Format::V16U16,
    Format::A2W10V10U10,

    Format::UYVY,
    Format::R8G8_B8G8,
    Format::YUY2,
    Format::G8R8_G8B8,
    Format::DXT1,
    Format::DXT2,
    Format::DXT3,
    Format::DXT4,
    Format::DXT5,

    Format::D16_LOCKABLE,
    Format::D32,
    Format::D15S1,
    Format::D24S8,
    Format::D24X8,
    Format::D24X4S4,
    Format::D16,

    Format::D32F_LOCKABLE,
    Format::D24FS8,

    Format::L16,

    Format::VERTEXDATA,
    Format::INDEX16,
    Format::INDEX32,

    Format::Q16W16V16U16,

    Format::MULTI2_ARGB8,

    Format::R16F,
    Format::G16R16F,
    Format::A16B16G16R16F,

    Format::R32F,
    Format::G32R32F,
    Format::A32B32G32R32F,

    Format::CxV8U8
};

std::string g_Strings[] =
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


std::string Format::ToString( Format::TYPE format )
{
    const TYPE * values = g_Values;
    const std::string * strings = g_Strings;
    
    for( int i = 0; ! strings[ i ].empty(); ++i )
    {
	    if ( values[ i ] == format )
	    {
		    return strings[ i ];
	    }
    }
    return "--ERROR--";
}

Format::TYPE Format::FromString( const std::string & string )
{
    const Format::TYPE * values = g_Values;
    const std::string * strings = g_Strings;
    
    for( int i = 0; ! strings[ i ].empty(); ++i )
    {
	    if ( stricmp( strings[ i ].c_str(), string.c_str() ) == 0 )
	    {
		    return values[ i ];
	    }
    }
    throw std::exception( "Failed to find Format string for conversion!" );
}

std::string Format::ToString( D3DFORMAT format )
{
    return ToString( static_cast< TYPE >( format ) );
}
