// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaRotation.h>
#include <me/dyna/IDynaDelta.h>

namespace me::dyna::rotation
{
	/// <summary>
	/// Applies a IDynaDelta to a IDynaRotation
	/// </summary>
	class Delta : public IDynaRotation
	{
		IDynaRotation::ptr m_rotation;
		IDynaDelta::ptr m_delta;

	public:
		Delta( IDynaRotation::ptr rotaiton, IDynaDelta::ptr delta );

		Result GetRotation( unify::Quaternion& out, unify::TimeDelta delta ) override;
	};
}