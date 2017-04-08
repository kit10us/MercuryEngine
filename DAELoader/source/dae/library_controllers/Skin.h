// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dae/library_geometries/Mesh.h>
#include <dae/DocumentNode.h>
#include <dae/library_effects/Effect.h>
#include <me/render/PrimitiveList.h>
#include <qxml/QXML.h>
#include <string>

namespace dae
{
	/// <summary>
	/// DAE node: COLLADA->library_controllers->controller->skin (1 or morph)
	/// </summary>
	class Skin : public DocumentNode
	{
	public:
		Skin( IDocument & document, const qxml::Element * node );
		std::string GetSource() const;

	private:
		std::string m_source;
	};
}
