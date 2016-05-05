/* Copyright (c) 2002 - 2014, Quentin S. Smith
 * All Rights Reserved
 */

template< typename T >
Optional< T >::Optional()
{
}

template< typename T >
Optional< T >::Optional( const T & thing )
	: m_thing( new T( thing ) )
{
}

template< typename T >
Optional< T >::~Optional()
{
	m_thing.reset();
}

template< typename T >
const T & Optional< T >::operator()
{
	if ( m_thing.empty() )
	{
		throw new unify::Exception( "Optional referenced when empty!" );
	}
	return m_thing;
}