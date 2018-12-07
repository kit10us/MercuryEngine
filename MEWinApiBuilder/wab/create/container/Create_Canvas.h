// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/container/Create_Container.h>
#include <list>

namespace create
{
	namespace container
	{
		class Canvas : public Container
		{
		public:
			Canvas( int width, int height );
			virtual ~Canvas();

			void AddChild( IControl * child ) override;

		private:
			void ComputePass1() override;
			void ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight ) override;
			void ComputePass3( int x, int y ) override;
			void Create( HWND parent ) override;

			std::list< IControl * > m_children;
		};
	}
}