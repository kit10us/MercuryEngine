// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <unify/TimeDelta.h>
#include <unify/Any.h>
#include <memory>
#include <map>

namespace me
{
	namespace render
	{
		/// <summary>
		/// This object is created per. geometry user (via Objects). It keeps specific data for
		/// geometry state and charateristics. It's specialized per. geometry. ie., a mesh would
		/// provide a specialized MeshInstanceData.
		/// This pushes all unique data from the geometry, to this object.
		/// </summary>
		class GeometryInstanceData
		{
		public:
			typedef std::shared_ptr< GeometryInstanceData > ptr;

			GeometryInstanceData();
			virtual ~GeometryInstanceData();

			virtual void Update( unify::Seconds elapsed );

			void SetMatrix( unify::Matrix matrix );
			const unify::Matrix * GetMatrix() const;
			bool IsMatrixSet() const;
			void ResetMatrix();

		private:
			bool m_isMatrixSet;
			unify::Matrix m_matrix;
		};
	}
}