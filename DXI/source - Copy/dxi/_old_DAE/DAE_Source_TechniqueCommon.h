// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/dae/DAE_Accessor.h>
#include <qxml/QXML.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_geometries->geometry->mesh->source->technique_common (0 or 1)
		/// </summary>
		class Source_TechniqueCommon
		{
		public:
			Source_TechniqueCommon( const qxml::Element * node );

			const boost::shared_ptr< Accessor > & GetAccessor() const;

		private:
			boost::shared_ptr< Accessor > m_accessor;
		};
	}
}
