template< typename T >
Key<T>::Key( unify::TimeDelta time, const T & value )
: m_time( time )
, m_value( value )
{
}

template< typename T >
bool Key<T>::operator<( const Key< T > & otherKey )
{
	return m_time < otherKey.m_time;
}

template< typename T >
bool Key<T>::operator<( unify::TimeDelta time ) const
{
	return m_time < time;
}

template< typename T >
unify::TimeDelta Key<T>::Time() const
{
	return m_time;
}

template< typename T >
const T & Key<T>::Value() const
{
	return m_value;
}

template< typename T >
void me::frameanimation::FindLeftRight( unify::TimeDelta time, const typename Key< T >::list & timeline, typename Key< T >::list::const_iterator & leftItr, typename Key< T >::list::const_iterator & rightItr )
{
	std::list< Key< T > >::const_iterator end = timeline.end();
	for( rightItr = timeline.begin(), leftItr = end; rightItr != end && (*rightItr) < time; ++rightItr )
	{
		leftItr = rightItr;
	}
}

template< typename T >
T me::frameanimation::InterpretValue( unify::TimeDelta time, const typename Key< T >::list & timeline, T defaultValue )
{
	if( timeline.empty() )
	{
		return defaultValue;
	}

	Key< T >::list::const_iterator leftItr;
	Key< T >::list::const_iterator rightItr;
	FindLeftRight< T >( time, timeline, leftItr, rightItr );

	// If our time is passed the last element, return the last element.
	// B<--------->Et
	if( rightItr == timeline.end() )
	{
		return leftItr->Value();
	}

	float mix;
	T leftValue;
	T rightValue;

	// If our time is before our first element, interpolate from defaultValue to first.
	// tB<--------->E
	if( leftItr == timeline.end() )
	{
		mix = time / rightItr->Time();
		leftValue = defaultValue;
		rightValue = rightItr->Value();
	}
	// If our time is between two elements, interpolate.
	// B<----t----->E
	else
	{
		mix = ( time - leftItr->Time() ) / ( rightItr->Time() - leftItr->Time() );
		leftValue = leftItr->Value();
		rightValue = rightItr->Value();
	}
	float invMix = 1.0f - mix;
	T final( (leftValue * invMix) + (rightValue * mix ) );
	return final;
}
