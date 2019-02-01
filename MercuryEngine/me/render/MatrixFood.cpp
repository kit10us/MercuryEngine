// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/MatrixFood.h>

using namespace me;
using namespace render;

MatrixFoodBase::MatrixFoodBase( size_t size )
	: m_size{ size }
{
}

size_t MatrixFoodBase::GetSize() const
{
	return m_size;
}



MatrixFood_InstancesSet::MatrixFood_InstancesSet( const InstancesSet * instancesList, size_t size  )
	: MatrixFoodBase( size )
	, instancesList{ instancesList }
{
}

void MatrixFood_InstancesSet::ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const
{
	while( majorIndex < GetSize() && read < max)
	{
		out[read++] = instancesList[ majorIndex ].instances[ minorIndex++].Final();
		if ( minorIndex >= instancesList[ majorIndex].size )
		{
			minorIndex = 0;
			majorIndex++;
		}
	}
}


MatrixFood_Matrices::MatrixFood_Matrices( const unify::Matrix * matrices, size_t size )
	: MatrixFoodBase( size )
	, matrices{ matrices }
{
}

void MatrixFood_Matrices::ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const
{
	while( majorIndex < GetSize() && read < max)
	{
		out[read++] = matrices[ majorIndex++ ];
	}
}


MatrixFood_Frames::MatrixFood_Frames( const unify::FrameLite ** instances, size_t size )
	: MatrixFoodBase( size )
	, instances{ instances }
{
}

void MatrixFood_Frames::ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const
{
	while( majorIndex < GetSize() && read < max)
	{
		instances[ majorIndex++]->ReadMatrix( &out[read++] );
	}
}



MatrixFood_IMatrixSource::MatrixFood_IMatrixSource( IMatrixSource * const * sources, size_t size )
	: MatrixFoodBase( size )
	, sources{ sources }
{
}

void MatrixFood_IMatrixSource::ReadMatrices( unify::Matrix * out, size_t & read, size_t max, size_t & minorIndex, size_t & majorIndex ) const
{
	while( majorIndex < GetSize() && read < max)
	{
		sources[ majorIndex ]->CopyMatrices( &out[read] );
		read += sources[ majorIndex++ ]->Count();
	}
}

