// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/VertexBuffer.h>
#include <dxi/dae/DAE_FloatArray.h>
#include <dxi/dae/DAE_Source_TechniqueCommon.h>
#include <dxi/dae/DAE_Exceptions.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <qxml/QXML.h>
#include <boost/optional.hpp>
#include <string>

namespace dxi
{
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
			FVF::TYPE GetFVF() const;

		private:
			std::string m_id;
			std::string m_name;
			boost::shared_ptr< FloatArray > m_float_array;
			boost::shared_ptr< Source_TechniqueCommon > m_technique_common;
			FVF::TYPE m_FVF;
		};
	}
}