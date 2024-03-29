// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaScale.h>
#include <me/dyna/IDynaDelta.h>

namespace me::dyna::scale
{
	/// <summary>
	/// Applies a IDynaDelta to a IDynaScale
	/// </summary>
	class Delta : public IDynaScale
	{
	public:
		Delta( IDynaScale::ptr scale, IDynaDelta::ptr delta );

		Result GetScale( unify::V3< float >& out, unify::TimeDelta delta ) override;

	private:
		IDynaScale::ptr m_scale;
		IDynaDelta::ptr m_delta;
	};
}