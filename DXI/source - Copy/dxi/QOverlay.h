/*
 * Copyright (c) 2002 Quentin S. Smith
 * All Rights Reserved
 *
 */
#pragma once

namespace dxi
{
	enum OverlayRegion 
	{
		Region_TopLeft,
		Region_Top,
		Region_TopRight,
		Region_Left,
		Region_Center,
		Region_Right,
		Region_BottomLeft,
		Region_Bottom,
		Region_BottomRight,
		Num_OverlayRegion
	};

	class QOverlay
	{
	public:
		QOverlay();
		~QOverlay();

		void Clear();
		void ClearTextRegion( OverlayRegion region );
		void AddText( OverlayRegion region, const std::string & sText );
	};

	#include "dxi/QOverlay.inl"
}