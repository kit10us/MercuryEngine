#pragma once

namespace dxi
{
	enum
	{
		COMMONVERTEXSHADER_NORMAL,
		COMMONVERTEXSHADER_CARTOON1,
		COMMONVERTEXSHADER_CARTOON2
	};

	const char * GetCommonShader( unsigned int dwShader )
	{
		switch( dwShader )
		{
		default:
			return NULL;

		case COMMONVERTEXSHADER_NORMAL:
			return
				// c0[4] - Combined Transformation Matrix
				// c4[1] - Directional Light
				"vs.1.1				\n"
				"dcl_position v0	\n"
				"dcl_normal0 v1		\n"
				"dcl_color0 v2		\n"
				"dcl_texcoord0 unify::V3	\n"

				"m4x4 oPos, v0, c0	\n"	// Transform Position

				"dp3 r0, v1, c4		\n"	// Lighting
				//"mul r0, r0, r0.w	\n"	// Intensify lighting
				"mul oD0, r0.x, v2	\n" // Color

				"mov oT0.xy, unify::V3		\n"	// Assign Texture Coordinates
			;

		case COMMONVERTEXSHADER_CARTOON1:
			return
				// c0[4] - Combined Transformation Matrix
				// c4[1] - Directional Light
				"vs.1.1				\n"
				"dcl_position v0	\n"
				"dcl_normal0 v1		\n"
				"dcl_color0 v2		\n"
				"dcl_texcoord0 unify::V3	\n"

				"def c5, 0.0f, 0.0f, 0.0f, 0.0f \n"	// Comparison
				"def c6, 0.25f, 0.25f, 0.25f, 0.25f \n" // low mul
				"def c7, 1.00f, 1.00f, 1.00f, 1.00f \n" // normal mul

				"m4x4 oPos, v0, c0	\n"	// Transform Position

				"dp3 r0, v1, c4		\n"	// Lighting
				
				////////////////////////
				// NORMAL METHOD...
				//"mul oD0, r0.x, v2	\n" // Color
				////////////////////////

				// Case of normal color...
				"sge r1, r0, c5		\n"	// r1 == 1 (if normal) else 0
				
				// NEED VS.2.0 for this...
				// "lrp r0, r1, c7, c6 \n"

				// So for now...
				"sub r2, c7, r1		\n"
				"mul r2, r2, c6		\n"
				"mad r0, r1, c7, r2 \n"

				"mul oD0, r0, v2 \n"

				"mov oT0.xy, unify::V3		\n"	// Assign Texture Coordinates
			;

		case COMMONVERTEXSHADER_CARTOON2:
			return
				// c0[4] - Combined Transformation Matrix
				// c4[1] - Directional Light
				"vs.1.1				\n"
				"dcl_position v0	\n"
				"dcl_normal0 v1		\n"
				"dcl_color0 v2		\n"
				"dcl_texcoord0 unify::V3	\n"

				"def c5, -0.50f, -0.50f, -0.50f, -0.50f \n"	// Comparison 1
				"def c6, 0.25f, 0.25f, 0.25f, 0.25f \n" // low mul
				"def c7, 1.00f, 1.00f, 1.00f, 1.00f \n" // normal mul
				"def c8, 0.5f, 0.5f, 0.5f, 0.5f \n"
				"def c9, 0.0f, 0.0f, 0.0f, 0.0f \n"

				"m4x4 oPos, v0, c0	\n"	// Transform Position

				"dp3 r0, v1, c4		\n"	// Lighting

				// Add steps of brightness...
				"mov r2, c6 \n"

				// Step 1
				"sge r1, r0, c5		\n"
				"mad r2, r1, c6, r2 \n"

				// Step 2
				"sge r1, r0, c9		\n"
				"mad r2, r1, c6, r2 \n"

				// Step 3
				"sge r1, r0, c8		\n"
				"mad r2, r1, c6, r2 \n"

				"mul oD0, r2, v2 \n"

				"mov oT0.xy, unify::V3		\n"	// Assign Texture Coordinates
			;
		}
	}
}