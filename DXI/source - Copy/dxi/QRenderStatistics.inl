inline void QRenderStatistics::Reset( unify::FrameID frameID )
{
	m_FrameID = frameID;
	for( unify::UInt32 u = 0; u < Num_Statistics; u++ ) m_Statistic[ u ] = 0;
}

inline void QRenderStatistics::Increment( RenderStatistic statistic, unify::Int32 iIncr )
{
	m_Statistic[ statistic ] += iIncr;
}

inline unify::Int32 QRenderStatistics::GetCount( RenderStatistic statistic ) const
{
	return m_Statistic[ statistic ];
}