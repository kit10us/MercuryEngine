// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <create/Create_Control.h>

namespace ui
{
	class Window;
}

namespace create
{
	namespace container
	{
		class Container : public Control
		{
			friend class ui::Window;
		protected:
			Container( int width, int height );

			void SetParent( Container * parent );

			Container * m_parent;
		public:
			virtual ~Container();

			virtual void AddChild( IControl * control ) = 0;
				
			Container * GetParent();

			DWORD GetWantedStyle() const override;
			std::wstring GetType() const override;
		};
	}
}