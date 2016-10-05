#pragma once

#include <string>
#include <dx/DX.h>

namespace dx
{
    struct Format
    {
        enum TYPE
        {
	        Unknown = D3DFMT_UNKNOWN,

	        R8G8B8 = D3DFMT_R8G8B8,
	        A8R8G8B8 = D3DFMT_A8R8G8B8,
	        X8R8G8B8 = D3DFMT_X8R8G8B8,
	        R5G6B5 = D3DFMT_R5G6B5,
	        X1R5G5B5 = D3DFMT_X1R5G5B5,
	        A1R5G5B5 = D3DFMT_A1R5G5B5,
	        A4R4G4B4 = D3DFMT_A4R4G4B4,
	        R3G3B2 = D3DFMT_R3G3B2,
	        A8 = D3DFMT_A8,
	        A8R3G3B2 = D3DFMT_A8R3G3B2,
	        X4R4G4B4 = D3DFMT_X4R4G4B4,
	        A2B10G10R10 = D3DFMT_A2B10G10R10,
	        A8B8G8R8 = D3DFMT_A8B8G8R8,
	        X8B8G8R8 = D3DFMT_X8B8G8R8,
	        G16R16 = D3DFMT_G16R16,
	        A2R10G10B10 = D3DFMT_A2R10G10B10,
	        A16B16G16R16 = D3DFMT_A16B16G16R16,

	        A8P8 = D3DFMT_A8P8,
	        P8 = D3DFMT_P8,

	        L8 = D3DFMT_L8,
	        A8L8 = D3DFMT_A8L8,
	        A4L4 = D3DFMT_A4L4,

	        V8U8 = D3DFMT_V8U8,
	        L6V5U5 = D3DFMT_L6V5U5,
	        X8L8V8U8 = D3DFMT_X8L8V8U8,
	        Q8W8V8U8 = D3DFMT_Q8W8V8U8,
	        V16U16 = D3DFMT_V16U16,
	        A2W10V10U10 = D3DFMT_A2W10V10U10,

	        UYVY = D3DFMT_UYVY,
	        R8G8_B8G8 = D3DFMT_R8G8_B8G8,
	        YUY2 = D3DFMT_YUY2,
	        G8R8_G8B8 = D3DFMT_G8R8_G8B8,
	        DXT1 = D3DFMT_DXT1,
	        DXT2 = D3DFMT_DXT2,
	        DXT3 = D3DFMT_DXT3,
	        DXT4 = D3DFMT_DXT4,
	        DXT5 = D3DFMT_DXT5,

	        D16_LOCKABLE = D3DFMT_D16_LOCKABLE,
	        D32 = D3DFMT_D32,
	        D15S1 = D3DFMT_D15S1,
	        D24S8 = D3DFMT_D24S8,
	        D24X8 = D3DFMT_D24X8,
	        D24X4S4 = D3DFMT_D24X4S4,
	        D16 = D3DFMT_D16,

	        D32F_LOCKABLE = D3DFMT_D32F_LOCKABLE,
	        D24FS8 = D3DFMT_D24FS8,

	        L16 = D3DFMT_L16,

	        VERTEXDATA = D3DFMT_VERTEXDATA,
	        INDEX16 = D3DFMT_INDEX16,
	        INDEX32 = D3DFMT_INDEX32,

	        Q16W16V16U16 = D3DFMT_Q16W16V16U16,

	        MULTI2_ARGB8 = D3DFMT_MULTI2_ARGB8,

	        R16F = D3DFMT_R16F,
	        G16R16F = D3DFMT_G16R16F,
	        A16B16G16R16F = D3DFMT_A16B16G16R16F,

	        R32F = D3DFMT_R32F,
	        G32R32F = D3DFMT_G32R32F,
	        A32B32G32R32F = D3DFMT_A32B32G32R32F,

	        CxV8U8 = D3DFMT_CxV8U8
        };
        
        static std::string ToString( TYPE format );
        static TYPE FromString( const std::string & string );
        static std::string ToString( D3DFORMAT format );
    };
}

