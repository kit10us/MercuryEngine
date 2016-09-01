// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/VertexBuffer.h>
#include <dae/FloatArray.h>
#include <dae/Source_TechniqueCommon.h>
#include <dae/Exceptions.h>
#include <dae/DocumentNode.h>
#include <qxml/QXML.h>
#include <string>

namespace dae
{
	/// <summary>
	/// DAE node: COLLADA->library_geometries->geometry->mesh->source (1 or more)
	/// </summary>
	class Source : public DocumentNode
	{
	public:
		Source( IDocument & document, const qxml::Element * node );

		const std::string & GetID() const;
		const std::string & GetName() const;
		const FloatArray & GetFloatArray() const;
		const Source_TechniqueCommon & GetTechniqueCommon() const;
		std::string GetSourceType() const;

	private:
		std::string m_id;
		std::string m_name;
		std::shared_ptr< FloatArray > m_float_array;
		std::shared_ptr< Source_TechniqueCommon > m_technique_common;
		std::string m_sourceType;
	};
}