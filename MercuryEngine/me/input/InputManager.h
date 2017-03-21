// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <qxml/QXML.h>
#include <unify/String.h>
#include <vector>
#include <map>

namespace me
{
	class IGame;

	namespace input
	{
		/// <summary>
		/// Managers all of our input sources.
		/// </summary>
		class InputManager
		{
		public:
			InputManager( IGame * game );
			~InputManager();

			void AddInputSource( IInputSource::ptr source );

			size_t GetSourceCount() const;
			IInputSource::ptr GetSource( size_t index ) const;
			IInputSource::ptr FindSource( std::string name );

			IInputCondition::ptr MakeCondition(const qxml::Element * element);

			/// <summary>
			/// Parse a node, whose children are "inputaction" nodes. Returns the number of failures.
			/// It will stop immediately upon a failure, if continueOnFail is false.
			/// </summary>
			size_t AddInputActions(unify::Owner::ptr owner, const qxml::Element * parentNode, bool continueOnFail );

			void Update();

			void Clear();

		private:
			bool AddSingleInputAction(unify::Owner::ptr owner, const qxml::Element * element);

			IGame* m_game;
			std::vector< IInputSource::ptr > m_sourceList;
			std::map< std::string, IInputSource::ptr, unify::CaseInsensitiveLessThanTest > m_sourceMap;
		};
	}
}