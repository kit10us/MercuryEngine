// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Matrix.h>
#include <unify/FrameSet.h>
#include <unify/TimeDelta.h>
#include <bitset>

namespace me
{
	namespace render
	{
		class IRenderer;

		// Use to track all rendering variables and needs, including current frame.
		class RenderInfo
		{
		public:
			RenderInfo();
			RenderInfo( const RenderInfo & info );
			~RenderInfo() throw ( );
			RenderInfo & operator=( const RenderInfo & info );

			void SetDistanceFromCamera( float distance );
			float DistanceFromCamera() const;

			unsigned int FrameID() const;
			void IncrementFrameID();

			unify::TimeDelta GetDelta() const;
			void SetDelta( unify::TimeDelta delta );

			unify::TimeDelta GetTotalDelta() const;

			float GetDeltaCount() const;

			unify::TimeDelta GetAverageDelta() const;

			float GetFPS() const;

			void SetViewMatrix( const unify::Matrix & matrix );
			const unify::Matrix & GetViewMatrix() const;

			void SetProjectionMatrix( const unify::Matrix & matrix );
			const unify::Matrix & GetProjectionMatrix() const;

			void SetFrameSetInstance( const unify::FrameSetInstance * frameSetInstance );
			const unify::FrameSetInstance * GetFrameSetInstance() const;

		protected:
			float m_distanceFromCamera;		// Distance from camera
			unsigned int m_frameID;
			unify::TimeDelta m_delta;
			unify::TimeDelta m_totalDelta;
			unsigned int m_deltaCount;
			unify::Matrix m_view;
			unify::Matrix m_projection;
			const unify::FrameSetInstance * m_frameSetInstance;
		};
	}
}