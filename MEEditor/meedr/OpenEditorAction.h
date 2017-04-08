// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MEEditor.h>
#include <me/action/IAction.h>

namespace meedr
{
	/// <summary>
	/// Open the editor.
	/// </summary>
	class OpenEditorAction : public me::action::IAction
	{
	public:
		OpenEditorAction();

	public: // IAction...
		bool Perform() override;
	};
}
