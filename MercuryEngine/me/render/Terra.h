// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

///<summary>
/// A 3D shape (flexible indexed primitives)
/// 
/// ShapeArgs is used to create shapes. It holds the values used when creating the shape.
///		ShapeArgs
///</summary>

#pragma once

#include <me/render/Geometry.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>
#include <me/render/Mesh.h>
#include <unify/RowColumn.h>
#include <unify/TexArea.h>
#include <unify/Parameters.h>
#include <unify/ColorUnit.h>
#include <unify/Range.h>

namespace me
{
	namespace render
	{
#define SIDE_NONE		FLAGNULL
#define SIDE_UP			FLAG01
#define SIDE_DOWN		FLAG02
#define SIDE_LEFT		FLAG03
#define SIDE_RIGHT		FLAG04

#define WRAP_NONE		FLAGNULL
#define WRAP_COLUMNS	FLAG01
#define WRAP_ROWS		FLAG02

#define TRANS_BELOW		FLAG01
#define TRANS_ABOVE		FLAG02


		// NOTE: column = uColumn-1 = DOWN/BOTTOM (towards)
		//		 column = 0 = UP/TOP (away)

		class Terra : public Mesh
		{
		public:
			struct TextureOpMap
			{
				TextureOpMap()
				{
				}

				TextureOpMap( ITexture::ptr texture, const unify::ColorUnit & colorOp, const unify::TexArea texArea = unify::TexAreaFull() )
					: texture( texture )
					, colorOp( colorOp )
					, texArea( texArea )
				{
				}

				ITexture::ptr texture;
				unify::ColorUnit colorOp;
				unify::TexArea texArea;
			};

			struct Parameters : public unify::Parameters
			{
				Parameters();
				Parameters( const unify::Size< float > size, const unify::RowColumn< unsigned int > faces, const float constant, const unify::TexArea texArea, Effect::ptr effect );
				void SetSize( const unify::Size< float > size );
				void SetFaces( const unify::RowColumn< unsigned int > faces );
				void SetPoints( const unify::RowColumn< unsigned int > points );
				void SetConstant( const float constant );
				void SetTexArea( const unify::TexArea texArea );
				void SetEffect( Effect::ptr effect );
				void SetHeightMap( Terra::TextureOpMap tom );
				void SetAlphaMap( Terra::TextureOpMap tom );
				void SetDiffuse( unify::ColorUnit diffuse );
				void SetDiffuses( unify::ColorUnit diffuseUL, unify::ColorUnit diffuseUR, unify::ColorUnit diffuseDL, unify::ColorUnit diffuseDR );
			};

			Terra( IRenderer * renderer );
			Terra( IRenderer * renderer, unify::Parameters & parameters );
			~Terra();

			void Destroy();

			void CreateFromParameters( unify::Parameters & parameters );

			// would be good to make these rely on a generic implementation (perhaps more complex with these as simplified front-ends)
			// from the Mesh geometry object.
			bool ApplyHeightMap( TextureOpMap tom );
			bool ApplyAlphaMap( TextureOpMap tom );
			bool ApplyTextureMap( unsigned int dwMember, const unify::TexArea * pTexArea );
			bool ApplyTransparent( unsigned int uFlags, float fValue, float fTolerance = 0.0f );
			bool Smooth( unsigned int uFlags = FLAGNULL );
			bool MakeWrappable( unsigned int uFlags = WRAP_COLUMNS | WRAP_ROWS );
			bool FixSide( unsigned int	uFlags, float fToDepth );
			bool AlignSide( unsigned int uFlags, Terra* pTerraIn );
			void Offset( const unify::V3< float > & vec );
			bool NormalSide( unsigned int uFlags, const unify::V3< float > & normal = unify::V3< float >( 0, 1, 0 ) );
			bool RenderNormals();

			/// <summary>
			/// Get the number of sample points. (faces + 1)
			/// </summmat>
			unify::RowColumn< unsigned int > GetPointCount() const;

			/// <summary>
			/// Get depth at a specific point between 0,0 and 1.0, 1.0.
			/// </summary>
			float GetDepth( float x, float y ) const;

			/// <summary>
			/// Get depth at a specific row and column; note that column is generally considered across the width or X coordinate.
			/// </summary>
			float GetDepth( unify::RowColumn< unsigned int > rc ) const;

			unify::Range< float > GetMinMax() const;

			unify::Size< float > GetSize() const;

		protected:
			unify::Parameters m_parameters;
			unify::Size< float > m_size;
			float * m_depth;
			unify::Range< float > m_minmax;
			unify::RowColumn< unsigned int > m_pointCount;
		};
	}
}