// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/V3.h>
#include <unify/Ray.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace unify
{
	/// <summary>
	/// A Bounding box.
	/// </summary>
	template< typename T = float >
	class BBox
	{
	public:
		V3< T > sup;
		V3< T > inf;

		BBox();
		BBox( const V3< T > & inf, const V3< T > & sup );

		BBox< T > operator * ( const V3< T > & muliplcand ) const;
		BBox< T > & operator *= ( const V3< T > & muliplcand );

		BBox< T > operator + ( const V3< T > & v3 ) const;
		BBox< T > & operator += ( const V3< T > & v3 );
		
		BBox< T > operator - ( const V3< T > & v3 ) const;
		BBox< T > & operator -= ( const V3< T > & v3 );

		void GenerateCorners( V3< T > * bounds );
		void Initialize( const V3< T > & inf = V3< T >::V3Zero(), const V3< T > & sup = V3< T >::V3Zero() );
		void AddPoint( const V3< T > & point );
		BBox & AddBBox( const BBox< T > & boundingBox );
		BBox & AddBBoxWithPosition( const BBox< T > & boundingBox, const V3< T > & position );  
		bool ContainsPoint( const V3< T > & point );
		const V3< T > Size() const;

        // Perform a hit test, return true if hit...
        bool RayTest( const Ray< T > & ray, T t0, T t1 ) const;
        
		// ... this version sets hitPoint to the first point of collision.
        bool RayTest( const Ray< T > & ray, V3< T > & hitPoint ) const;

	private:
		void Fix(); // Ensures our sup is superior and our inf, inferior.
	};
	#include <unify/BBox.inl>
}
