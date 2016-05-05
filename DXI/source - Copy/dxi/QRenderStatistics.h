/*
*/
#pragma once

#include "unify/Unify.h"

namespace dxi
{
	typedef enum RenderStatistic
	{
		Statistic_SetLight,
		Statistic_SetRenderState,
		Statistic_SetTransform,
		Statistic_PrimitiveListRenderCalls,
		Statistic_SetTexture,
		Statistic_UnsetTexture,
		Num_Statistics
	};


	class QRenderStatistics
	{
	public:
		QRenderStatistics();
		~QRenderStatistics();

		void Reset( unify::FrameID frameID );
		void Increment( RenderStatistic statistic, unify::Int32 iIncr = 1 );
		unify::Int32 GetCount( RenderStatistic statistic ) const;

	protected:
		unify::FrameID m_FrameID;
		unify::Int32 m_Statistic[ Num_Statistics ];
	};

	#include "dxi/QRenderStatistics.inl"
}