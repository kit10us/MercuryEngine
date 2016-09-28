// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>
#include <unify/unify.h>

namespace dxi
{
	/*
	// Render states..
	struct RenderState
	{
		enum TYPE
		{
			ZEnable = D3DRS_ZENABLE,
			FillMode = D3DRS_FILLMODE,					// See FillModeValues below.
			ShadowMode = D3DRS_SHADEMODE,
			ZWriteEnable = D3DRS_ZWRITEENABLE,
			AlphaTestEnable = D3DRS_ALPHATESTENABLE,
			LastPixel = D3DRS_LASTPIXEL,
			SrcBlend = D3DRS_SRCBLEND,
			DestBlend = D3DRS_DESTBLEND,
			CullMode = D3DRS_CULLMODE,					// See CullModeValues below.
			ZFunc = D3DRS_ZFUNC,
			AlphaRef = D3DRS_ALPHAREF,
			AlphaFunc = D3DRS_ALPHAFUNC,
			DitherEnable = D3DRS_DITHERENABLE,
			AlphaBlendEnable = D3DRS_ALPHABLENDENABLE,
			FogEnable = D3DRS_FOGENABLE,
			SpecularEnable = D3DRS_SPECULARENABLE,
			FogColor = D3DRS_FOGCOLOR,
			FogTableMode = D3DRS_FOGTABLEMODE,
			FogStart = D3DRS_FOGSTART,
			FogEnd = D3DRS_FOGEND,
			FogDensity = D3DRS_FOGDENSITY,
			RangeFogEnable = D3DRS_RANGEFOGENABLE,
			StencilEnable = D3DRS_STENCILENABLE,
			StencilFail = D3DRS_STENCILFAIL,
			StencilZFail = D3DRS_STENCILZFAIL,
			StencilPass = D3DRS_STENCILPASS,
			StencilFunc = D3DRS_STENCILFUNC,
			StencilRef = D3DRS_STENCILREF,
			StencilMask = D3DRS_STENCILMASK,
			StencilWriteMask = D3DRS_STENCILWRITEMASK,
			TextureFactor = D3DRS_TEXTUREFACTOR,
			Wrap0 = D3DRS_WRAP0,
			Wrap1 = D3DRS_WRAP1,
			Wrap2 = D3DRS_WRAP2,
			Wrap3 = D3DRS_WRAP3,
			Wrap4 = D3DRS_WRAP4,
			Wrap5 = D3DRS_WRAP5,
			Wrap6 = D3DRS_WRAP6,
			Wrap7 = D3DRS_WRAP7,
			Clipping = D3DRS_CLIPPING,
			Lighting = D3DRS_LIGHTING,
			Ambient = D3DRS_AMBIENT,
			FoxVertexMode = D3DRS_FOGVERTEXMODE,
			ColorVertex = D3DRS_COLORVERTEX,
			LocalViewer = D3DRS_LOCALVIEWER,
			NormalizeNormals = D3DRS_NORMALIZENORMALS,

			// Obsolete due to deprecation of Fixed Function Pipeline.
			// DiffuseMaterialSource = D3DRS_DIFFUSEMATERIALSOURCE,
			// SpecularMaterialSource = D3DRS_SPECULARMATERIALSOURCE,
			// AmbientMaterialSource = D3DRS_AMBIENTMATERIALSOURCE,
			// EmissibeMaterialSource = D3DRS_EMISSIVEMATERIALSOURCE,

			VertexBlend = D3DRS_VERTEXBLEND,
			ClipPlaneEnable = D3DRS_CLIPPLANEENABLE,
			PointSize = D3DRS_POINTSIZE,
			PointSizeMin = D3DRS_POINTSIZE_MIN,
			PointSpriteEnable = D3DRS_POINTSPRITEENABLE,
			PointScaleEnable = D3DRS_POINTSCALEENABLE,
			PointScaleA = D3DRS_POINTSCALE_A,
			PointScaleB = D3DRS_POINTSCALE_B,
			PointScaleC = D3DRS_POINTSCALE_C,
			MultiSampleAntiAlias = D3DRS_MULTISAMPLEANTIALIAS,
			MultiSampleMask = D3DRS_MULTISAMPLEMASK,
			PatchEdgeStyle = D3DRS_PATCHEDGESTYLE,
			DebugMonitorToken = D3DRS_DEBUGMONITORTOKEN,
			PointSizeMax = D3DRS_POINTSIZE_MAX,
			IndexedVertexBlendEnable = D3DRS_INDEXEDVERTEXBLENDENABLE,
			ColorWriteEnable = D3DRS_COLORWRITEENABLE,
			TweenFactgor = D3DRS_TWEENFACTOR,
			BlendOp = D3DRS_BLENDOP,
			PositionDegree = D3DRS_POSITIONDEGREE,
			NormalDegree = D3DRS_NORMALDEGREE,
			ScissorTestEnable = D3DRS_SCISSORTESTENABLE,
			SlopeScaleDepthBias = D3DRS_SLOPESCALEDEPTHBIAS,
			AntiAliasedLineEnable = D3DRS_ANTIALIASEDLINEENABLE,
			MinTessellationLevel = D3DRS_MINTESSELLATIONLEVEL,
			MaxTessellationLevel = D3DRS_MAXTESSELLATIONLEVEL,
			AdaptiveTessX = D3DRS_ADAPTIVETESS_X,
			AdaptiveTessY = D3DRS_ADAPTIVETESS_Y,
			AdaptiveTessZ = D3DRS_ADAPTIVETESS_Z,
			AdaptiveTessW = D3DRS_ADAPTIVETESS_W,
			EnableAdaptiveTessellation = D3DRS_ENABLEADAPTIVETESSELLATION,
			TwoSideStencilMode = D3DRS_TWOSIDEDSTENCILMODE,
			CCWStencilFail = D3DRS_CCW_STENCILFAIL,
			CCWStencilZFail = D3DRS_CCW_STENCILZFAIL,
			CCWStencilPass = D3DRS_CCW_STENCILPASS,
			CCWStencilFunc = D3DRS_CCW_STENCILFUNC,
			ColorWriteEnable1 = D3DRS_COLORWRITEENABLE1,
			ColorWriteEnable2 = D3DRS_COLORWRITEENABLE2,
			ColorWriteEnable3 = D3DRS_COLORWRITEENABLE3,
			BlendFactor = D3DRS_BLENDFACTOR,
			SRGBWriteEnable = D3DRS_SRGBWRITEENABLE,
			DepthBias = D3DRS_DEPTHBIAS,
			Wrap8 = D3DRS_WRAP8,
			Wrap9 = D3DRS_WRAP9,
			Wrap10 = D3DRS_WRAP10,
			Wrap11 = D3DRS_WRAP11,
			Wrap12 = D3DRS_WRAP12,
			Wrap13 = D3DRS_WRAP13,
			Wrap14 = D3DRS_WRAP14,
			Wrap15 = D3DRS_WRAP15,
			SeperateAlphaBlendEnable = D3DRS_SEPARATEALPHABLENDENABLE,
			SrcBlendAlpha = D3DRS_SRCBLENDALPHA,
			DestBlendAlpha = D3DRS_DESTBLENDALPHA,
			BlendOpAlpha = D3DRS_BLENDOPALPHA,
		};

		struct FillModeValues
		{
			enum
			{
				Point = D3DFILL_POINT,
				WireFrame = D3DFILL_WIREFRAME,
			    Solid = D3DFILL_SOLID
			};
		};

		struct CullModeValues
		{
			enum
			{
				None = D3DCULL_NONE,
				Clockwise	= D3DCULL_CW,
				CounteClockwise = D3DCULL_CCW
			};
		};

		struct LightingValues
		{
			enum
			{
				Enabled = 1,
				Disabled = 0
			};
		};

		struct ColorVertexValues
		{
			enum
			{
				Enabled = 1,
				Disabled = 0
			};
		};

		struct ZEnableValues
		{
			enum
			{
				False = D3DZB_FALSE,
				True = D3DZB_TRUE,
				UseW = D3DZB_USEW
			};
		};

		static void Set( RenderState::TYPE, unsigned long value );
		static unsigned long Get( RenderState::TYPE );
	};
	*/
}