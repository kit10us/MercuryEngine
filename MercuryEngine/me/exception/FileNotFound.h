// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>
#include <unify/Path.h>

namespace me
{
	namespace exception
	{
		class FileNotFound : public unify::Exception
		{
			unify::Path m_path;
		public:
			FileNotFound( unify::Path path )
				: unify::Exception( "File \"" + path.ToString() + "\" not found!" )
				, m_path{ path }
			{
			}
			
			unify::Path GetPath() const
			{
				return m_path;
			}
		};
	} 
}