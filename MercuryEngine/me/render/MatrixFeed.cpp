// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/render/MatrixFeed.h>

using namespace me;
using namespace render;

MatrixFeed::MatrixFeed()
	: m_phase{ Phase::InstancesSetPhase }
	, m_stride{ 1 }

	, m_major_index{ 0 }
	, m_minor_index{ 0 }
{
}

MatrixFeed::MatrixFeed( MatrixFood_InstancesSet food, size_t stride )
	: m_phase{ Phase::InstancesSetPhase }
	, m_stride{ stride }	
	, m_major_index{ 0 }
	, m_minor_index{ 0 }

	, m_instancesSet_food{ food }
{
}

MatrixFeed::MatrixFeed( MatrixFood_Matrices food, size_t stride )
	: m_phase{ Phase::InstancesSetPhase }
	, m_stride{ stride }
	, m_major_index{ 0 }
	, m_minor_index{ 0 }

	, m_matrices_food{ food }
{
}

MatrixFeed::MatrixFeed( MatrixFood_Frames food, size_t stride )
	: m_phase{ Phase::InstancesSetPhase }
	, m_stride{ stride }
	, m_major_index{ 0 }
	, m_minor_index{ 0 }

	, m_frames_food{ food }
{
}

MatrixFeed::MatrixFeed( MatrixFood_IMatrixSource food, size_t stride )
	: m_phase{ Phase::InstancesSetPhase }
	, m_stride{ stride }
	, m_major_index{ 0 }
	, m_minor_index{ 0 }

	, m_matrixsource_food { food }
{
}

size_t MatrixFeed::Consume( unify::Matrix * out, size_t max )
{
	size_t read = 0;

	// Align to the amount of reads/writes we actually want.
	max = max - (max % m_stride );

	while( m_phase != Phase::Done )
	{
		// Determine our current phase...
		if( m_phase == Phase::InstancesSetPhase &&  m_major_index >= m_instancesSet_food.size )
		{
			m_major_index = 0;
			m_phase = Phase::MatrixPointerPhase;
		}
		if( m_phase == Phase::MatrixPointerPhase && m_major_index >= m_matrices_food.size )
		{
			m_major_index = 0;
			m_phase = Phase::FramePointerPhase;
		}
		if( m_phase == Phase::FramePointerPhase && m_major_index >= m_frames_food.size )
		{
			m_major_index = 0;
			m_phase = Phase::IMatrixSourcePhase;
		}
		if( m_phase == Phase::IMatrixSourcePhase && m_major_index >= m_matrixsource_food.size )
		{
			m_major_index = 0;
			m_phase = Phase::Done;
		}

		switch( m_phase )
		{
		case Phase::InstancesSetPhase:
			while( m_major_index < m_instancesSet_food.size && read < max )
			{
				auto && instances = m_instancesSet_food.instancesList[m_major_index];
				const FrameAndMatrix & fm = instances.instances[m_minor_index++];
				out[read++] = fm.Final();
				if( m_minor_index >= instances.size )
				{
					m_minor_index = 0;
					m_major_index++;
				}
			}
			break;
		case Phase::MatrixPointerPhase:
			while( m_major_index < m_matrices_food.size && read < max )
			{
				out[read++] = m_matrices_food.matrices[m_major_index++];
			}
			break;
		case Phase::FramePointerPhase:
			while( m_major_index < m_frames_food.size && read < max )
			{
				m_frames_food.instances[m_major_index++]->ReadMatrix( &out[read++] );
			}
			break;
		case Phase::IMatrixSourcePhase:
			while( m_major_index < m_matrixsource_food.size && read < max )
			{
				m_matrixsource_food.sources[m_major_index]->CopyMatrices( &out[read] );
				read += m_matrixsource_food.sources[m_major_index++]->Count();
			}
			break;
		}

		if( m_phase == Phase::Done || read == max )
		{
			return read;
		}
	}

	return 0;
}

size_t MatrixFeed::Stride() const
{
	return m_stride;
}

bool MatrixFeed::Done() const
{
	return m_phase == Phase::Done;
}

void MatrixFeed::Restart()
{
	m_phase = Phase::InstancesSetPhase;
	m_major_index = 0;
	m_minor_index = 0;
	m_stride = 1;
}