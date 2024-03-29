// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/render/Skeleton.h>
#include <me/render/Mesh.h>

using namespace me;
using namespace render;

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::AddJoint( std::string name, unify::Matrix matrix, std::string parent )
{
	size_t index = m_jointMatrix.size();
	m_jointMatrix.push_back( matrix );
	m_names[name] = index;
	if( parent.empty() )
	{
		m_parent.push_back( Root );
		m_jointFinalMatrix.push_back( matrix );
	}
	else
	{
		size_t parentIndex = FindJointIndex( parent );
		m_parent.push_back( parentIndex );
		m_jointFinalMatrix.push_back( matrix * m_jointFinalMatrix[ parentIndex ] );
	}
}

size_t Skeleton::FindJointIndex( std::string name ) const
{
	auto itr = m_names.find( name );
	if( itr == m_names.end() )
	{
		return Root;
	}
	else
	{
		return itr->second;
	}
}

const unify::Matrix * Skeleton::FindJoint( std::string name ) const
{
	size_t index = FindJointIndex( name );
	if( index < m_jointMatrix.size() )
	{
		return &m_jointMatrix[index];
	}

	return nullptr;
}

bool Skeleton::Empty() const
{
	return m_jointMatrix.empty();
}

void Skeleton::Render( const render::Params & params, render::MatrixFeed & matrixFeed, Effect::ptr effect, float size, unify::Color color )
{
	if( Empty() ) return;

	unify::Matrix matrix;
	while( matrixFeed.Consume( &matrix, 1 ) )
	{
		for( size_t i = 0; i < m_jointFinalMatrix.size(); i++ )
		{
			unify::Matrix matrixFinal =	
				m_jointFinalMatrix[i] 
				* matrix;
			render::MatrixFeed feed( render::MatrixFood_Matrices{ &matrixFinal, 1 }, 1);
			m_jointGeo->Render( params, nullptr, feed );
		}
	}
	matrixFeed.Restart();
}
