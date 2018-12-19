// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputDevice.h>
#include <me/UpdateParams.h>
#include <qxml/QXML.h>
#include <unify/String.h>
#include <vector>
#include <map>

namespace me
{
	namespace game
	{
		class IGame;
	}

	namespace input
	{
		/// <summary>
		/// Managers all of our input sources.
		/// </summary>
		class InputManager
		{
		public:
			InputManager( game::IGame * game );
			~InputManager();

			void AddInputDevice( IInputDevice::ptr source );

			size_t GetSourceCount() const;
			IInputDevice::ptr GetSource( size_t index ) const;
			IInputDevice::ptr FindSource( std::string name );

			IInputCondition::ptr MakeCondition(const qxml::Element * element);

			/// <summary>
			/// Parse a node, whose children are "inputaction" nodes. Returns the number of failures.
			/// It will stop immediately upon a failure, if continueOnFail is false.
			/// </summary>
			size_t AddInputActions(unify::Owner::ptr owner, const qxml::Element * parentNode, bool continueOnFail );

			void Update( const UpdateParams & params );

			void Clear();

		private:
			bool AddSingleInputAction(unify::Owner::ptr owner, const qxml::Element * element);

			game::IGame * m_game;
			std::vector< IInputDevice::ptr > m_sourceList;
			std::map< std::string, IInputDevice::ptr, unify::string::CaseInsensitiveLessThanTest > m_sourceMap;
		};
	}
}