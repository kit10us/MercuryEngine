/* Copyright (c) 2002 - 2014, Quentin S. Smith
 * All Rights Reserved
 */

template< typename T >
MinMax< T >::MinMax()
: m_isSet( false )
{
}

template< typename T >
unify::MinMax< T >::MinMax( T min, T max )
: m_isSet( true )
, m_min( min )
, m_max( min )
{
	Add( max );
}

template< typename T >
MinMax< T >::MinMax( const MinMax & minMax )
: m_isSet( minMax.m_isSet )
, m_min( minMax.m_min )
, m_max( minMax.m_max )
{
}

template< typename T >
MinMax< T >::MinMax( const std::string & text )
{
	std::vector< T > split = Split< T >( text, ',' );
    width = split[ 0 ];
    height = split[ 1 ];
}

template< typename T >
void unify::MinMax< T >::Add( T value )
{
	if( m_isSet )
	{
		m_min = std::min<>( m_min, value );
		m_max = std::max<>( m_max, value );
	}
	else
	{
		m_min = value;
		m_max = value;
		m_isSet = true;
	}
}

template< typename T >
bool unify::MinMax< T >::IsSet() const
{
	return m_isSet;
}

template< typename T >
T unify::MinMax< T >::Min() const
{
	assert( m_isSet );
	return m_min;
}

template< typename T >
T unify::MinMax< T >::Max() const
{
	assert( m_isSet );
	return m_max;
}

template< typename T >
bool MinMax< T >::IsWithin( const T & value ) const
{
	return value >= Min() && value <= Max();
}

template< typename T >
MinMax< T > & MinMax< T >::operator=( const MinMax< T > & minMax )
{
	m_isSet = minMax.m_isSet;
	m_min = minMax.m_min;
	m_max = minMax.m_max;
	return *this;
}

template< typename T >
bool unify::MinMax< T >::operator==( const MinMax< T > & minMax ) const
{
	return m_isSet == minMax.m_isSet && m_min == minMax.m_min && m_max == minMax.m_max;
}

template< typename T >
bool unify::MinMax< T >::operator!=( const MinMax< T > & minMax ) const
{
	return !( *this == minMax );
}