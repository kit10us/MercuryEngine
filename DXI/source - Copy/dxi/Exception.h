#pragma once

// Using macros to prevent creation of "what" if there is no failure.

#define OnFailedThrow( hr, what )	\
	if ( FAILED( hr ) ) throw unify::Exception( what )

#define OnFalseThrow( check, what )	\
	if ( ! check ) throw unify::Exception( what )