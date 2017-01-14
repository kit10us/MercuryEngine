// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/FrameSet.h>

namespace unify
{
	/// <summary>
	/// A modified FrameSet. Used with animations, separating animation changes into FrameSetInstance, while maintaining the original FrameSet.
	/// </summary>
	class FrameSetInstance
	{
	public:
		FrameSetInstance( const FrameSet & frameSet );
		FrameSetInstance( const FrameSetInstance & frameSetInstance );
		~FrameSetInstance();

		FrameSetInstance & operator=( const FrameSetInstance & frameSetInstance );

		size_t Count() const;
		void Reset();
		bool Exists( const std::string & name ) const;
		size_t Find( const std::string & name ) const;
		const Matrix & Model( size_t index ) const;
		const Matrix & Local( size_t index ) const;
		const Matrix & OriginalModel( size_t ) const;
		const Matrix & OriginalLocal( size_t ) const;
		void Transform( size_t index, const Matrix & transform );
		void UpdateLocals();

	private:
		const FrameSet & m_frameSet;
		std::vector< Matrix > m_models;
		std::vector< Matrix > m_locals; // local = parent * model
		bool m_dirty;
	};
}