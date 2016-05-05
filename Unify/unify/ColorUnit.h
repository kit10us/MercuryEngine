// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <string>

namespace unify
{
	class Color;

	// A color whose components are normalized (0.0 to 1.0) at standard/real ranges.
	class ColorUnit
	{
	public:
		enum Order {
			RGBA,
			ARGB,
			BGRA,
			ABGR,
		};

		// Named constructors...
		static ColorUnit ColorUnitRGBA( float r, float g, float b, float a );
		static ColorUnit ColorUnitARGB( float a, float r, float g, float b );
		static ColorUnit ColorUnitRGB( float r, float g, float b );
		static ColorUnit ColorUnitWhite( float a = 1.0f );
		static ColorUnit ColorUnitRed( float r = 1.0f, float a = 1.0f );
		static ColorUnit ColorUnitGreen( float g = 1.0f, float a = 1.0f );
		static ColorUnit ColorUnitBlue( float b = 1.0f, float a = 1.0f );
		static ColorUnit ColorUnitGrey( float grey, float a = 1.0f );
		static ColorUnit ColorUnitBlack( float a = 1.0f);
		static ColorUnit ColorUnitZero();

        float r, g, b, a;

		ColorUnit();
		ColorUnit( float r, float g, float b, float a );
		ColorUnit( const ColorUnit & color );
		explicit ColorUnit( const std::string & text, Order order = RGBA, float defaultAlpha = 1.0f );

		void SetRGBA( float r, float g, float b, float a );
		void Normalize();

		// CONVERSIONS
		operator Color();

		// Combines the various components into a single scalar using addition: result = a + r + g + b;
		float SumComponents();

		// OPERATOR OVERLOADING MEMBER FUNCTIONS
		ColorUnit & operator += ( const ColorUnit & col );
		ColorUnit & operator -= ( const ColorUnit & col );
		ColorUnit & operator *= ( float val );
		ColorUnit & operator /= ( float  val );

		// unary operators
		//V3 operator + () const;
		//V3 operator - () const;

		// binary operators
		ColorUnit operator + ( const ColorUnit & col ) const;
		ColorUnit operator - ( const ColorUnit & col ) const;
		ColorUnit operator * ( const ColorUnit & col ) const;
		ColorUnit operator / ( const ColorUnit & col ) const;
		ColorUnit operator * ( float val ) const;
		ColorUnit operator / ( float val ) const;

		//friend V3 operator * ( float, const struct V3& );

		bool operator == ( const ColorUnit & col ) const;
		bool operator != ( const ColorUnit & col ) const;

		void SetRed( float r );
		void SetGreen( float g );
		void SetBlue( float b );
		void SetAlpha( float a );

		std::string ToString( Order order = RGBA ) const;
	};
} // namespace unify
