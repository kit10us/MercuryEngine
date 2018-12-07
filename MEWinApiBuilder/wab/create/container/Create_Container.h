// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>

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
			std::string m_tag;

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

			Container * SetDebugTag( std::string tag )
			{
				m_tag = tag;
				return this;
			}

		};
	}
}