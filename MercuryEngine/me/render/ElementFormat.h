// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>
#include <unify/Matrix.h>
#include <string>

namespace me
{
	namespace render
	{
		struct ElementFormat
		{
			enum TYPE
			{
				Float1,
				Float2,
				Float3,
				Float4,
				Int1,
				Int2,
				Int3,
				Int4,
				Matrix4x4,
				ColorUNorm,
				Unknown
			};

			static size_t SizeOf( TYPE format );
			static TYPE FromString( std::string format );
			static std::string ToString( TYPE format );
			static bool Convert( TYPE formatOut, void * out, TYPE formatIn, const void * in );

			template< typename T >
			struct FromType
			{
			};

			template<>
			struct FromType< float >
			{
				operator TYPE() { return Float1; }
			};

			template<>
			struct FromType< unify::V2< float > >
			{
				operator TYPE() { return Float2; }
			};

			template<>
			struct FromType< unify::V3< float > >
			{
				operator TYPE() { return Float3; }
			};

			template<>
			struct FromType< unify::V4< float > >
			{
				operator TYPE() { return Float4; }
			};

			template<>
			struct FromType< unify::TexCoords >
			{
				operator TYPE() { return Float2; }
			};

			template<>
			struct FromType< int >
			{
				operator TYPE() { return Int1; }
			};

			template<>
			struct FromType< unify::V2< int > >
			{
				operator TYPE() { return Int2; }
			};

			template<>
			struct FromType< unify::V3< int > >
			{
				operator TYPE() { return Int3; }
			};

			template<>
			struct FromType< unify::V4< int > >
			{
				operator TYPE() { return Int4; }
			};

			template<>
			struct FromType< unify::Color >
			{
				operator TYPE() { return ColorUNorm; }
			};

			template<>
			struct FromType< unify::ColorUnit >
			{
				operator TYPE() { return Float4; }
			};

			template<>
			struct FromType< unify::Matrix >
			{
				operator TYPE() { return Matrix4x4; }
			};

		};
	}
}