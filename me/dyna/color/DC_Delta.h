// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/dyna/IDynaColor.h>
#include <me/dyna/IDynaDelta.h>

namespace me::dyna::color
{
	/// <summary>
	/// Applies a IDynaDelta to a IDynaColor
	/// </summary>
	class Delta : public IDynaColor
	{
		IDynaColor::ptr m_color;
		IDynaDelta::ptr m_delta;

	public:
		Delta( IDynaColor::ptr rotaiton, IDynaDelta::ptr delta );

	public: // me::dyna::color::IDynaColor...
		Result GetColor(unify::ColorUnit & out, unify::TimeDelta delta) override;
	};
}
