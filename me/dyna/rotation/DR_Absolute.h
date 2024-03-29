// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaRotation.h>

namespace me::dyna::rotation
{
	/// <summary>
	/// An absolute rotation.
	/// </summary>
	class Absolute : public IDynaRotation
	{
		unify::Quaternion m_q;

	public:
		Absolute( unify::Quaternion q );

		Result GetRotation( unify::Quaternion& out, unify::TimeDelta delta ) override;
	};
}