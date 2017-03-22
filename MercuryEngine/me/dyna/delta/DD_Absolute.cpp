// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/dyna/delta/DD_Absolute.h>

using namespace me;
using namespace dyna;
using namespace delta;

Absolute::Absolute( float delta )
	: m_delta{ delta }
{
}

Result Absolute::GetDelta( float & delta )
{
	delta = m_delta;
	return Result::Failure;
}
