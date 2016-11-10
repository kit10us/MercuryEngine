#include <me/frameanimation/FrameAnimationKey.h>

template<>
unify::Quaternion me::frameanimation::InterpretValue< unify::Quaternion >( float time, const typename RotationKey::list & timeline, unify::Quaternion defaultValue )
{
	if( timeline.empty() )
	{
		return defaultValue;
	}

	RotationKey::list::const_iterator leftItr;
	RotationKey::list::const_iterator rightItr;
	FindLeftRight< unify::Quaternion >( time, timeline, leftItr, rightItr );

	// If our time is passed the last element, return the last element.
	// B<--------->Et
	if( rightItr == timeline.end() )
	{
		return leftItr->Value();
	}

	float mix;
	unify::Quaternion leftValue;
	unify::Quaternion rightValue;

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
	unify::Quaternion final( unify::QuaternionSlerp( leftValue, rightValue, mix ) );
	return final;
}
