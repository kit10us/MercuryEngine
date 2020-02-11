// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <unify/FrameLite.h>

namespace me
{
	namespace render
	{
		/// <summary>
		/// Used to track an object's frame, and it's geometries' model's matrix.
		/// </summary>
		struct FrameAndMatrix
		{
			const unify::FrameLite * frame;
			const unify::Matrix * matrix;

			unify::Matrix Final() const
			{
				return matrix ? ( *matrix * frame->GetMatrix() ) : frame->GetMatrix();
			}
		};

		struct InstancesSet
		{
			InstancesSet( const FrameAndMatrix * instances, const size_t size )
			: instances{ instances }
			, size{ size }
			{
			}

			const FrameAndMatrix * instances;
			const size_t size;
		};
	}
}