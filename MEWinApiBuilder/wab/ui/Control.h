// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/IControl.h>

namespace create
{
	class IControl;
}

namespace ui
{				 
	class Control : public IControl
	{
		friend class Window;

	protected:
		Control( HWND parent, HWND handle, create::IControl * createControl );

		HWND m_parentHandle;
		HWND m_handle;
		create::IControl * m_createControl;

	public:
		virtual ~Control();

		HWND GetParentHandle() const override;
		HWND GetHandle() const override;

		std::string GetName() const override;
		int GetID() const override;

	public: // WinApi functions...
		void SetText( std::string text ) override;
		void AppendText( std::string text );
		std::string GetText() const override;
		bool GetEnabled() const override;
		void SetEnable( bool enable ) override;
	};
}