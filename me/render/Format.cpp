// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/Format.h>
#include <unify/String.h>

using namespace me;
using namespace render;

Format::TYPE Format::FromString( std::string format )
{
	using unify::string::StringIs;

	if ( StringIs( format, "Unknown" ) ) { return Format::Unknown; }
	else if ( StringIs( format, "R32G32B32A32_Typeless" ) ) { return Format::R32G32B32A32_Typeless; }
	else if ( StringIs( format, "R32G32B32A32_Float" ) ) { return Format::R32G32B32A32_Float; }
	else if ( StringIs( format, "R32G32B32A32_UInt" ) ) { return Format::R32G32B32A32_UInt; }
	else if ( StringIs( format, "R32G32B32A32_SInt" ) ) { return Format::R32G32B32A32_SInt; }
	else if ( StringIs( format, "R32G32B32_Typeless" ) ) { return Format::R32G32B32_Typeless; }
	else if ( StringIs( format, "R32G32B32_Float" ) ) { return Format::R32G32B32_Float; }
	else if ( StringIs( format, "R32G32B32_UInt" ) ) { return Format::R32G32B32_UInt; }
	else if ( StringIs( format, "R32G32B32_SInt" ) ) { return Format::R32G32B32_SInt; }
	else if ( StringIs( format, "R16G16B16A16_Typeless" ) ) { return Format::R16G16B16A16_Typeless; }
	else if ( StringIs( format, "R16G16B16A16_Float" ) ) { return Format::R16G16B16A16_Float; }
	else if ( StringIs( format, "R16G16B16A16_UNorm" ) ) { return Format::R16G16B16A16_UNorm; }
	else if ( StringIs( format, "R16G16B16A16_UInt" ) ) { return Format::R16G16B16A16_UInt; }
	else if ( StringIs( format, "R16G16B16A16_SNorm" ) ) { return Format::R16G16B16A16_SNorm; }
	else if ( StringIs( format, "R16G16B16A16_SInt" ) ) { return Format::R16G16B16A16_SInt; }
	else if ( StringIs( format, "R32G32_Typeless" ) ) { return Format::R32G32_Typeless; }
	else if ( StringIs( format, "R32G32_Float" ) ) { return Format::R32G32_Float; }
	else if ( StringIs( format, "R32G32_UInt" ) ) { return Format::R32G32_UInt; }
	else if ( StringIs( format, "R32G32_SInt" ) ) { return Format::R32G32_SInt; }
	else if ( StringIs( format, "R32G8X24_Typeless" ) ) { return Format::R32G8X24_Typeless; }
	else if ( StringIs( format, "D32_Float_S8X24_UInt" ) ) { return Format::D32_Float_S8X24_UInt; }
	else if ( StringIs( format, "R32_Float_X8X24_Typeless" ) ) { return Format::R32_Float_X8X24_Typeless; }
	else if ( StringIs( format, "X32_Typeless_G8X24_UInt" ) ) { return Format::X32_Typeless_G8X24_UInt; }
	else if ( StringIs( format, "R10G10B10A2_Typeless" ) ) { return Format::R10G10B10A2_Typeless; }
	else if ( StringIs( format, "R10G10B10A2_UNorm" ) ) { return Format::R10G10B10A2_UNorm; }
	else if ( StringIs( format, "R10G10B10A2_UInt" ) ) { return Format::R10G10B10A2_UInt; }
	else if ( StringIs( format, "R11G11B10_Float" ) ) { return Format::R11G11B10_Float; }
	else if ( StringIs( format, "R8G8B8A8_Typeless" ) ) { return Format::R8G8B8A8_Typeless; }
	else if ( StringIs( format, "R8G8B8A8_UNorm" ) ) { return Format::R8G8B8A8_UNorm; }
	else if ( StringIs( format, "R8G8B8A8_UNorm_SRGB" ) ) { return Format::R8G8B8A8_UNorm_SRGB; }
	else if ( StringIs( format, "R8G8B8A8_UInt" ) ) { return Format::R8G8B8A8_UInt; }
	else if ( StringIs( format, "R8G8B8A8_SNorm" ) ) { return Format::R8G8B8A8_SNorm; }
	else if ( StringIs( format, "R8G8B8A8_SINT" ) ) { return Format::R8G8B8A8_SInt; }
	else if ( StringIs( format, "R16G16_Typeless" ) ) { return Format::R16G16_Typeless; }
	else if ( StringIs( format, "R16G16_Float" ) ) { return Format::R16G16_Float; }
	else if ( StringIs( format, "R16G16_UNorm" ) ) { return Format::R16G16_UNorm; }
	else if ( StringIs( format, "R16G16_UInt" ) ) { return Format::R16G16_UInt; }
	else if ( StringIs( format, "R16G16_SNorm" ) ) { return Format::R16G16_SNorm; }
	else if ( StringIs( format, "R16G16_SINT" ) ) { return Format::R16G16_SInt; }
	else if ( StringIs( format, "R32_Typeless" ) ) { return Format::R32_Typeless; }
	else if ( StringIs( format, "D32_Float" ) ) { return Format::D32_Float; }
	else if ( StringIs( format, "R32_Float" ) ) { return Format::R32_Float; }
	else if ( StringIs( format, "R32_UInt" ) ) { return Format::R32_UInt; }
	else if ( StringIs( format, "R32_SInt" ) ) { return Format::R32_SInt; }
	else if ( StringIs( format, "R24G8_Typeless" ) ) { return Format::R24G8_Typeless; }
	else if ( StringIs( format, "D24_UNorm_S8_UInt" ) ) { return Format::D24_UNorm_S8_UInt; }
	else if ( StringIs( format, "R24_UNorm_X8_Typeless" ) ) { return Format::R24_UNorm_X8_Typeless; }
	else if ( StringIs( format, "X24_Typeless_G8_UInt" ) ) { return Format::X24_Typeless_G8_UInt; }
	else if ( StringIs( format, "R8G8_Typeless" ) ) { return Format::R8G8_Typeless; }
	else if ( StringIs( format, "R8G8_UNorm" ) ) { return Format::R8G8_UNorm; }
	else if ( StringIs( format, "R8G8_UInt" ) ) { return Format::R8G8_UInt; }
	else if ( StringIs( format, "R8G8_SNorm" ) ) { return Format::R8G8_SNorm; }
	else if ( StringIs( format, "R8G8_SInt" ) ) { return Format::R8G8_SInt; }
	else if ( StringIs( format, "R16_Typeless" ) ) { return Format::R16_Typeless; }
	else if ( StringIs( format, "R16_Float" ) ) { return Format::R16_Float; }
	else if ( StringIs( format, "D16_UNorm" ) ) { return Format::D16_UNorm; }
	else if ( StringIs( format, "R16_UNorm" ) ) { return Format::R16_UNorm; }
	else if ( StringIs( format, "R16_UInt" ) ) { return Format::R16_UInt; }
	else if ( StringIs( format, "R16_SNorm" ) ) { return Format::R16_SNorm; }
	else if ( StringIs( format, "R16_SInt" ) ) { return Format::R16_SInt; }
	else if ( StringIs( format, "R8_Typeless" ) ) { return Format::R8_Typeless; }
	else if ( StringIs( format, "R8_UNorm" ) ) { return Format::R8_UNorm; }
	else if ( StringIs( format, "R8_UInt" ) ) { return Format::R8_UInt; }
	else if ( StringIs( format, "R8_SNorm" ) ) { return Format::R8_SNorm; }
	else if ( StringIs( format, "R8_SInt" ) ) { return Format::R8_SInt; }
	else if ( StringIs( format, "A8_UNorm" ) ) { return Format::A8_UNorm; }
	else if ( StringIs( format, "R1_UNorm" ) ) { return Format::R1_UNorm; }
	else if ( StringIs( format, "R9G9B9E5_SharedExp" ) ) { return Format::R9G9B9E5_SharedExp; }
	else if ( StringIs( format, "R8G8_B8G8_UNorm" ) ) { return Format::R8G8_B8G8_UNorm; }
	else if ( StringIs( format, "G8R8_G8B8_UNorm" ) ) { return Format::G8R8_G8B8_UNorm; }
	else if ( StringIs( format, "BC1_Typeless" ) ) { return Format::BC1_Typeless; }
	else if ( StringIs( format, "BC1_UNorm" ) ) { return Format::BC1_UNorm; }
	else if ( StringIs( format, "BC2_Typeless" ) ) { return Format::BC2_Typeless; }
	else if ( StringIs( format, "BC2_UNorm" ) ) { return Format::BC2_UNorm; }
	else if ( StringIs( format, "BC2_UNorm_SRGB" ) ) { return Format::BC2_UNorm_SRGB; }
	else if ( StringIs( format, "BC3_Typeless" ) ) { return Format::BC3_Typeless; }
	else if ( StringIs( format, "BC3_UNorm" ) ) { return Format::BC3_UNorm; }
	else if ( StringIs( format, "BC3_UNorm_SRGB" ) ) { return Format::BC3_UNorm_SRGB; }
	else if ( StringIs( format, "BC4_Typeless" ) ) { return Format::BC4_Typeless; }
	else if ( StringIs( format, "BC4_UNorm" ) ) { return Format::BC4_UNorm; }
	else if ( StringIs( format, "BC4_SNorm" ) ) { return Format::BC4_SNorm; }
	else if ( StringIs( format, "BC5_Typeless" ) ) { return Format::BC5_Typeless; }
	else if ( StringIs( format, "BC5_UNorm" ) ) { return Format::BC5_UNorm; }
	else if ( StringIs( format, "BC5_SNorm" ) ) { return Format::BC5_SNorm; }
	else if ( StringIs( format, "B5G6R5_UNorm" ) ) { return Format::B5G6R5_UNorm; }
	else if ( StringIs( format, "B5G5R5A1_UNorm" ) ) { return Format::B5G5R5A1_UNorm; }
	else if ( StringIs( format, "B8G8R8A8_UNorm" ) ) { return Format::B8G8R8A8_UNorm; }
	else if ( StringIs( format, "B8G8R8X8_UNorm" ) ) { return Format::B8G8R8X8_UNorm; }
	else if ( StringIs( format, "R10G10B10_XR_BIAS_A2_UNorm" ) ) { return Format::R10G10B10_XR_BIAS_A2_UNorm; }
	else if ( StringIs( format, "B8G8R8A8Typeless" ) ) { return Format::B8G8R8A8Typeless; }
	else if ( StringIs( format, "B8G8R8A8UNormSRGB" ) ) { return Format::B8G8R8A8UNormSRGB; }
	else if ( StringIs( format, "B8G8R8X8Typeless" ) ) { return Format::B8G8R8X8Typeless; }
	else if ( StringIs( format, "B8G8R8X8UNormSRGB" ) ) { return Format::B8G8R8X8UNormSRGB; }
	else if ( StringIs( format, "BC6H_Typeless" ) ) { return Format::BC6H_Typeless; }
	else if ( StringIs( format, "BC6H_UF16" ) ) { return Format::BC6H_UF16; }
	else if ( StringIs( format, "BC6H_SF16" ) ) { return Format::BC6H_SF16; }
	else if ( StringIs( format, "BC7_Typless" ) ) { return Format::BC7_Typless; }
	else if ( StringIs( format, "BC7_UNorm" ) ) { return Format::BC7_UNorm; }
	else if ( StringIs( format, "BC7_UNormSRGB" ) ) { return Format::BC7_UNormSRGB; }
	else if ( StringIs( format, "AYUV" ) ) { return Format::AYUV; }
	else if ( StringIs( format, "Y410" ) ) { return Format::Y410; }
	else if ( StringIs( format, "Y416" ) ) { return Format::Y416; }
	else if ( StringIs( format, "NV12" ) ) { return Format::NV12; }
	else if ( StringIs( format, "P010" ) ) { return Format::P010; }
	else if ( StringIs( format, "P016" ) ) { return Format::P016; }
	else if ( StringIs( format, "Opaque420" ) ) { return Format::Opaque420; }
	else if ( StringIs( format, "YUY2" ) ) { return Format::YUY2; }
	else if ( StringIs( format, "Y210" ) ) { return Format::Y210; }
	else if ( StringIs( format, "Y216" ) ) { return Format::Y216; }
	else if ( StringIs( format, "NV11" ) ) { return Format::NV11; }
	else if ( StringIs( format, "AI44" ) ) { return Format::AI44; }
	else if ( StringIs( format, "IA44" ) ) { return Format::IA44; }
	else if ( StringIs( format, "P8" ) ) { return Format::P8; }
	else if ( StringIs( format, "A8P8" ) ) { return Format::A8P8; }
	else if ( StringIs( format, "B4G4R4A4UNorm" ) ) { return Format::B4G4R4A4UNorm; }
	
	throw unify::Exception( "Invalid Format \"" + format + "\"!" );
}

std::string Format::ToString( TYPE format )
{
	switch( format )
	{
	case Unknown: return "Unknown";
	case R32G32B32A32_Typeless: return "R32G32B32A32_Typeless";
	case R32G32B32A32_Float: return "R32G32B32A32_Float";
	case R32G32B32A32_UInt: return "R32G32B32A32_UInt";
	case R32G32B32A32_SInt: return "R32G32B32A32_SInt";
	case R32G32B32_Typeless: return "R32G32B32_Typeless";
	case R32G32B32_Float: return "R32G32B32_Float";
	case R32G32B32_UInt: return "R32G32B32_UInt";
	case R32G32B32_SInt: return "R32G32B32_SInt";
	case R16G16B16A16_Typeless: return "R16G16B16A16_Typeless";
	case R16G16B16A16_Float: return "R16G16B16A16_Float";
	case R16G16B16A16_UNorm: return "R16G16B16A16_UNorm";
	case R16G16B16A16_UInt: return "R16G16B16A16_UInt";
	case R16G16B16A16_SNorm: return "R16G16B16A16_SNorm";
	case R16G16B16A16_SInt: return "R16G16B16A16_SInt";
	case R32G32_Typeless: return "R32G32_Typeless";
	case R32G32_Float: return "R32G32_Float";
	case R32G32_UInt: return "R32G32_UInt";
	case R32G32_SInt: return "R32G32_SInt";
	case R32G8X24_Typeless: return "R32G8X24_Typeless";
	case D32_Float_S8X24_UInt: return "D32_Float_S8X24_UInt";
	case R32_Float_X8X24_Typeless: return "R32_Float_X8X24_Typeless";
	case X32_Typeless_G8X24_UInt: return "X32_Typeless_G8X24_UInt";
	case R10G10B10A2_Typeless: return "R10G10B10A2_Typeless";
	case R10G10B10A2_UNorm: return "R10G10B10A2_UNorm";
	case R10G10B10A2_UInt: return "R10G10B10A2_UInt";
	case R11G11B10_Float: return "R11G11B10_Float";
	case R8G8B8A8_Typeless: return "R8G8B8A8_Typeless";
	case R8G8B8A8_UNorm: return "R8G8B8A8_UNorm";
	case R8G8B8A8_UNorm_SRGB: return "R8G8B8A8_UNorm_SRGB";
	case R8G8B8A8_UInt: return "R8G8B8A8_UInt";
	case R8G8B8A8_SNorm: return "R8G8B8A8_SNorm";
	case R8G8B8A8_SInt: return "R8G8B8A8_SInt";
	case R16G16_Typeless: return "R16G16_Typeless";
	case R16G16_Float: return "R16G16_Float";
	case R16G16_UNorm: return "R16G16_UNorm";
	case R16G16_UInt: return "R16G16_UInt";
	case R16G16_SNorm: return "R16G16_SNorm";
	case R16G16_SInt: return "R16G16_SInt";
	case R32_Typeless: return "R32_Typeless";
	case D32_Float: return "D32_Float";
	case R32_Float: return "R32_Float";
	case R32_UInt: return "R32_UInt";
	case R32_SInt: return "R32_SInt";
	case R24G8_Typeless: return "R24G8_Typeless";
	case D24_UNorm_S8_UInt: return "D24_UNorm_S8_UInt";
	case R24_UNorm_X8_Typeless: return "R24_UNorm_X8_Typeless";
	case X24_Typeless_G8_UInt: return "X24_Typeless_G8_UInt";
	case R8G8_Typeless: return "R8G8_Typeless";
	case R8G8_UNorm: return "R8G8_UNorm";
	case R8G8_UInt: return "R8G8_UInt";
	case R8G8_SNorm: return "R8G8_SNorm";
	case R8G8_SInt: return "R8G8_SInt";
	case R16_Typeless: return "R16_Typeless";
	case R16_Float: return "R16_Float";
	case D16_UNorm: return "D16_UNorm";
	case R16_UNorm: return "R16_UNorm";
	case R16_UInt: return "R16_UInt";
	case R16_SNorm: return "R16_SNorm";
	case R16_SInt: return "R16_SInt";
	case R8_Typeless: return "R8_Typeless";
	case R8_UNorm: return "R8_UNorm";
	case R8_UInt: return "R8_UInt";
	case R8_SNorm: return "R8_SNorm";
	case R8_SInt: return "R8_SInt";
	case A8_UNorm: return "A8_UNorm";
	case R1_UNorm: return "R1_UNorm";
	case R9G9B9E5_SharedExp: return "R9G9B9E5_SharedExp";
	case R8G8_B8G8_UNorm: return "R8G8_B8G8_UNorm";
	case G8R8_G8B8_UNorm: return "G8R8_G8B8_UNorm";
	case BC1_Typeless: return "BC1_Typeless";
	case BC1_UNorm: return "BC1_UNorm";
	case BC2_Typeless: return "BC2_Typeless";
	case BC2_UNorm: return "BC2_UNorm";
	case BC2_UNorm_SRGB: return "BC2_UNorm_SRGB";
	case BC3_Typeless: return "BC3_Typeless";
	case BC3_UNorm: return "BC3_UNorm";
	case BC3_UNorm_SRGB: return "BC3_UNorm_SRGB";
	case BC4_Typeless: return "BC4_Typeless";
	case BC4_UNorm: return "BC4_UNorm";
	case BC4_SNorm: return "BC4_SNorm";
	case BC5_Typeless: return "BC5_Typeless";
	case BC5_UNorm: return "BC5_UNorm";
	case BC5_SNorm: return "BC5_SNorm";
	case B5G6R5_UNorm: return "B5G6R5_UNorm";
	case B5G5R5A1_UNorm: return "B5G5R5A1_UNorm";
	case B8G8R8A8_UNorm: return "B8G8R8A8_UNorm";
	case B8G8R8X8_UNorm: return "B8G8R8X8_UNorm";
	case R10G10B10_XR_BIAS_A2_UNorm: return "R10G10B10_XR_BIAS_A2_UNorm";
	case B8G8R8A8Typeless: return "B8G8R8A8Typeless";
	case B8G8R8A8UNormSRGB: return "B8G8R8A8UNormSRGB";
	case B8G8R8X8Typeless: return "B8G8R8X8Typeless";
	case B8G8R8X8UNormSRGB: return "B8G8R8X8UNormSRGB";
	case BC6H_Typeless: return "BC6H_Typeless";
	case BC6H_UF16: return "BC6H_UF16";
	case BC6H_SF16: return "BC6H_SF16";
	case BC7_Typless: return "BC7_Typless";
	case BC7_UNorm: return "BC7_UNorm";
	case BC7_UNormSRGB: return "BC7_UNormSRGB";
	case AYUV: return "AYUV";
	case Y410: return "Y410";
	case Y416: return "Y416";
	case NV12: return "NV12";
	case P010: return "P010";
	case P016: return "P016";
	case Opaque420: return "Opaque420";
	case YUY2: return "YUY2";
	case Y210: return "Y210";
	case Y216: return "Y216";
	case NV11: return "NV11";
	case AI44: return "AI44";
	case IA44: return "IA44";
	case P8: return "P8";
	case A8P8: return "A8P8";
	case B4G4R4A4UNorm: return "B4G4R4A4UNorm";
	}

	throw unify::Exception( "Invalid Format!" );
}
