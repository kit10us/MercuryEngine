// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/FVF.h>
#include <qxml/QXML.h>
#include <boost/optional.hpp>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_geometries->geometry (1 or more)
		/// </summary>
		class Input_Shared
		{
		public:
			Input_Shared( const qxml::Element * node );

			const int GetOffset() const;
			const std::string & GetSemantic() const;
			const std::string & GetSource() const;
			const boost::optional< int > & GetSet() const;
			const FVF::TYPE GetFVFType() const;
			size_t GetStride() const;
			
		private:
			int m_offset;
			std::string m_semantic;
			std::string m_source;
			boost::optional< int > m_set;
			FVF::TYPE m_fvfType;
			size_t m_stride;
		};
	}
}
