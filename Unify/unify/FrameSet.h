// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Matrix.h>
#include <map>

namespace unify
{
	///	<summary>
	/// A FrameSet is a set of matrices, and connectivity between these matrices, used for mesh animations.
	/// </summary>
	class FrameSet
	{
		friend class FrameSetInstance;
	public:
		class ForEachFunctor
		{
		public:
			virtual ~ForEachFunctor() {}
			virtual void operator()( size_t depth, size_t index ) = 0;
		};


		// Takes ownership...
		FrameSet();
		~FrameSet();

		size_t Add( const Matrix & model, size_t parent, const std::string & name = std::string() );
		bool Exists( const std::string & name ) const;
		size_t Find( const std::string & name ) const;
		size_t Count() const;
		std::string Name( size_t index ) const;
		const Matrix & Model( size_t index ) const;
		const Matrix & Local( size_t index ) const;
		void ForEach( ForEachFunctor & functor ) const;

	private:
		std::vector< size_t > m_parentIndex;
		std::vector< Matrix > m_models;
		std::vector< Matrix > m_locals; // local = parent * model
		std::map< std::string, size_t > m_nameToIndex;
		std::map< size_t, std::string > m_indexToName;
	};
}