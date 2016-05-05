// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/dae/DAE_Library.h>
#include <dxi/dae/library_visual_scenes/DAE_VisualScene.h>
#include <qxml/QXML.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_visual_scenes (0 or more)
		/// </summary>
		class LibraryVisualScenes : public Library< VisualScene >
		{
		public:
			LibraryVisualScenes( IDocument & document, const qxml::Element * node );
		};
	}
}
