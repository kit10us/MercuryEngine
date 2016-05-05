#pragma once

#include <boost/filesystem/path.hpp>

namespace dxi
{
	namespace xfile
	{
		class Element
		{
		public:
			Element();
			~Element();
			Element & operator++();
		protected:
		};

		class DirectXFile2
		{
		public:
			DirectXFile2( boost::filesystem::path & path );
			Element & GetRoot();
		protected:
		};
	}
}