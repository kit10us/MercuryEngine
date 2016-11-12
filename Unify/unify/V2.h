// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/String.h>

namespace unify
{
	template < class T > class V3;
	template < class T > class Size;

	template < class T = float >
	class V2
	{
	public:
		T x, y;

		V2();
		V2( T x, T y );
		V2( const V2< T > & vec );
		explicit V2( const Size< T > & size );
        explicit V2( const std::string & text );
		
		// Conversion operators...
		V2< T > & operator = ( const Size< T > & size );
		V2< T > & operator = ( const V3< T > & vec3 );

		// binary operators
		V2< T > & operator = ( const V2< T > & vec );

		V2< T > & operator += ( const V2< T > & vec );
		V2< T > & operator -= ( const V2< T > & vec );
		V2< T > & operator *= ( const V2< T > & vec );
		V2< T > & operator /= ( const V2< T > & vec );
		V2< T > & operator *= ( T val );
		V2< T > & operator /= ( T val );

		V2< T > operator + ( const V2< T > & vec) const;
		V2< T > operator - ( const V2< T > & vec ) const;
		V2< T > operator * ( const V2< T > & vec ) const;
		V2< T > operator / ( const V2< T > & vec ) const;
		V2< T > operator * ( T val ) const;
		V2< T > operator / ( T val ) const;

		bool operator == ( const V2< T > & vec ) const;
		bool operator != ( const V2< T > & vec ) const;

        T & operator[]( size_t i );
        const T & operator[]( size_t i ) const;

		T Length() const;
		void Normalize();
		bool IsZero() const;

        std::string ToString() const;

		// Named constructors
		static V2< T > V2Zero();
	};

	class V2F : public V2< float >
	{
	public:
		V2F();
	};
	
	#include <unify/V2.inl>
}
