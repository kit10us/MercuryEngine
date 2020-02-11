// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/MatrixFeed.h>

using namespace me;
using namespace render;

MatrixFeed::MatrixFeed( const MatrixFoodBase & food, size_t stride )
	: m_done{ false }
	, m_stride{ stride }	
	, m_major_index{ 0 }
	, m_minor_index{ 0 }
	, m_food{ &food }
{
}

size_t MatrixFeed::Consume( unify::Matrix * out, size_t max )
{
	size_t read = 0;

	// Align to the amount of reads/writes we actually want.
	max = max - (max % m_stride );

	while( ! m_done && read != max )
	{
		if ( m_major_index >= m_food->GetSize() )
		{
			m_major_index = 0;
			m_done = true;
		}
		else
		{
			m_food->ReadMatrices( out, read, max, m_minor_index, m_major_index );
		}
	}

	return read;
}

size_t MatrixFeed::Stride() const
{
	return m_stride;
}

bool MatrixFeed::Done() const
{
	return m_done;
}

void MatrixFeed::Restart()
{
	m_done = false;
	m_major_index = 0;
	m_minor_index = 0;
	m_stride = 1;
}