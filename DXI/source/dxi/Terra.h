// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A 3D shape (flexible indexed primitives)
/// 
/// ShapeArgs is used to create shapes. It holds the values used when creating the shape.
///		ShapeArgs
///</summary>

#pragma once

#include <dxi/Geometry.h>
#include <unify/RowColumn.h>
#include <unify/TexArea.h>
#include <unify/Parameters.h>
#include <unify/ColorUnit.h>

namespace dxi
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

	class Terra : public Geometry
	{
	public:
		struct TextureOpMap
		{
			TextureOpMap( Texture::ptr texture, const unify::ColorUnit & colorOp, const unify::TexArea texArea = unify::TexArea::Full() )
				: texture( texture )
				, colorOp( colorOp )
				, texArea( texArea )
			{
			}

			Texture::ptr texture;
			unify::ColorUnit colorOp;
			unify::TexArea texArea;
		};

		struct Parameters : public unify::Parameters
		{
			Parameters()
			{
			}

			Parameters( const unify::Size< float > size, const unify::RowColumn< unsigned int > rc, const float constant, const unify::TexArea texArea, Effect::ptr effect )
			{
				Set( "size", size );
				Set( "effect", effect ); 
				Set< unify::RowColumn< unsigned int > >( "rowscolumns", rc );
				Set< float >( "constant", constant );
				Set< unify::TexArea >( "texarea", texArea );
			}

			void SetSize( const unify::Size< float > size )
			{
				Set( "size", size );
			}
			void SetRowsColumns( const unify::RowColumn< unsigned int > rc )
			{
				Set< unify::RowColumn< unsigned int > >( "rowscolumns", rc );
			}
			void SetConstant( const float constant )
			{
				Set( "constant", constant );
			}
			void SetTexArea( const unify::TexArea texArea )
			{
				Set( "texarea", texArea );
			}
			void SetEffect( Effect::ptr effect )
			{
				Set( "effect", effect ); 
			}
			void SetHeightMap( Terra::TextureOpMap tom )
			{
				Set( "heightmap", tom );
			}
			void SetAlphaMap( Terra::TextureOpMap tom )
			{
				Set( "alphamap", tom );
			}
		};

		Terra();
		Terra( unify::Parameters & parameters );
		~Terra();

		void Destroy();

		virtual const unify::BBox< float > & ComputeBounds();
		void CreateFromParameters( unify::Parameters & parameters );
		void Update( unify::Seconds elapsed, GeometryInstanceData * instanceData );
		void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );

		// would be good to make these rely on a generic implementation (perhaps more complex with these as simplified front-ends)
		// from the Mesh geometry object.
		bool ApplyHeightMap( TextureOpMap tom );
		bool ApplyAlphaMap( TextureOpMap tom );
		bool ApplyTextureMap( unsigned int dwMember, const unify::TexArea * pTexArea );
		bool ApplyTransparent( unsigned int uFlags, float fValue, float fTolerance = 0.0f );
		void GenerateNormals( bool bCrossSide = false );
		bool Smooth( unsigned int uFlags = FLAGNULL );
		bool MakeWrappable( unsigned int uFlags = WRAP_COLUMNS | WRAP_ROWS );
		bool FixSide( unsigned int	uFlags, float fToDepth );
		bool AlignSide( unsigned int uFlags, Terra* pTerraIn );
		void Offset( const unify::V3< float > & vec );
		bool NormalSide( unsigned int uFlags, const unify::V3< float > & normal = unify::V3< float >(0,1,0) );
		bool RenderNormals();
		const unify::RowColumn< unsigned int > & GetRC() const;
		PrimitiveList & GetPrimitiveList();

	protected:
		unify::Parameters m_parameters;
		PrimitiveList m_primitiveList;
		std::vector< float > m_depth;
		unify::RowColumn< unsigned int > m_rc;
	};
}