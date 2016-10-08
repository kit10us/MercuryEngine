/* Copyright (c) 2002 - 2014, Quentin S. Smith
 * All Rights Reserved
 */
#pragma once

#include <unify/Unify.h>
#include <unify/String.h>
#include <algorithm>
#include <cassert>

namespace unify
{
	template< typename T >
	class MinMax
	{
	public:
		MinMax();
		MinMax( T min, T max );
		MinMax( const MinMax & minMax );
		explicit MinMax( const std::string & text );

		void Add( T value );
		bool IsSet() const;
		T Min() const;
		T Max() const;

		/// <summary>
		/// Test if a value is within the min and max range.
		/// </summary>
		bool IsWithin( const T & value ) const;

		MinMax & operator=( const MinMax & minMax );
		bool operator==( const MinMax & minMax ) const;
		bool operator!=( const MinMax & minMax ) const;
	private:
		bool m_isSet;
		T m_min, m_max;
	};
	#include <unify/MinMax.inl>
}
