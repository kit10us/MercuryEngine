// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/container/Create_Container.h>
#include <list>

namespace create
{
	namespace container
	{
		enum class Stack
		{
			Horizontal, Vertical
		};

		class StackPanel : public Container
		{
		public:
			StackPanel( Stack direction, FillWidth, FillHeight, int padding = 3 );
			StackPanel( Stack direction, FillWidth, SizeToContentHeight, int padding = 3 );
			StackPanel( Stack direction, FillWidth, int height, int padding = 3 );

			StackPanel( Stack direction, SizeToContentWidth, FillHeight, int padding = 3 );
			StackPanel( Stack direction, SizeToContentWidth, SizeToContentHeight, int padding = 3 );
			StackPanel( Stack direction, SizeToContentWidth, int height, int padding = 3 );

			StackPanel( Stack direction, int width, FillHeight, int padding = 3 );
			StackPanel( Stack direction, int width, SizeToContentHeight, int padding = 3 );
			StackPanel( Stack direction, int width, int height, int padding = 3 );		


			virtual ~StackPanel();

			Stack GetDirection() const;

			void AddChild( IControl * child );

			int GetPadding() const;
			void SetPadding( int padding );

		private:
			void ComputePass1() override;
			void ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight ) override;
			void ComputePass3( int x, int y ) override;
			void Create( HWND parent ) override;

			Stack m_direction;
			int m_padding;
			std::list< IControl * > m_children;
			int m_totalChildrenWidth;
			int m_totalChildrenHeight;
			int m_totalChildrenFillWidthWeight;
			int m_totalChildrenFillHeightWeight;
		};
	}
}
