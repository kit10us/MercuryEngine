// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/spline/ISpline.h>
#include <vector>
#include <memory>

namespace sculpter
{
	namespace spline
	{
		class SplineList
		{
		public:
			SplineList();
			SplineList( std::initializer_list< ISpline::ptr > splines );
			~SplineList();

			void AddSpline( ISpline::ptr spline );

			size_t GetSplineCount() const;
			const ISpline::ptr GetSpline( size_t index ) const;

			ISpline::ptr operator[]( size_t index );
			const ISpline::ptr operator[]( size_t index ) const;

		private:
			std::vector< ISpline::ptr > m_splines;
		};
	}
}