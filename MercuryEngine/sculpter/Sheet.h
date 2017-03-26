// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <sculpter/Point.h>
#include <sculpter/spline/SplineList.h>
#include <me/PrimitiveList.h>
#include <unify/Size.h>
#include <array>

namespace sculpter
{
	class Sheet
	{
	public:
		typedef std::shared_ptr< Sheet > ptr;

		Sheet( Point origin );
		Sheet( Point origin, spline::SplineList list, unify::Size< size_t > faceCount );
		~Sheet();

		void SetOrigin( Point origin );
		Point GetOrigin() const;

		void SetFaceCount( unify::Size< size_t > count );

		bool IsComplete() const;

		unify::Size< size_t > GetVertexCount() const;
		unify::Size< size_t > GetEdgeCount() const;
		unify::Size< size_t > GetFaceCount() const;

		spline::ISpline::ptr GetSpline( size_t index );
		size_t GetSplineCount() const;

		bool Create( me::PrimitiveList & primitiveList, unify::Size< float > size, me::Effect::ptr ) const;

	private:
		Point m_origin;
		unify::Size< size_t > m_faceCount;
		spline::SplineList m_splines;
	};
}
