// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputType.h>
#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <unify/Owner.h>
#include <memory>
#include <string>

namespace me
{
	namespace input
	{
		/// <summary>
		/// Simple states, when we need to test an input, this allows us to say a test is Invalid, or Valind and True or False.
		/// </summary>
		enum class State
		{
			Invalid = -1,
			False = 0,
			True = 1
		};

		/// <summary>
		/// A source of inputs.
		/// </summary>
		class IInputDevice
		{
		public:
			typedef std::shared_ptr< IInputDevice > ptr;

			virtual ~IInputDevice() {}

			/// <summary>
			/// Name of our source.
			/// </summaty>
			virtual std::string GetName() const = 0;

			/// <summary>
			/// Update called once a game cycle to enable us to update and poll our inputs.
			/// </summary>
			virtual void Update() = 0;

			/// <summary>
			/// Number of sub-sources. For example, gamepads can have four sub-sources, one for each controller.
			/// </summary>
			virtual size_t SubSourceCount() const = 0;

			/// <summary>
			/// Number of inputs available from this source.
			/// </summary>
			virtual size_t GetInputCount( size_t subSource ) const = 0;

			/// <summary>
			/// Get the name for an input by index.
			/// </summary>
			virtual std::string GetInputName( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Get the index of an input by name.
			/// </summary>
			virtual size_t GetInputIndex( size_t subSource, std::string name ) const = 0;

			/// <summary>
			/// Returns the type of an input, by index.
			/// </summary>
			virtual InputType GetInputType( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Get an input, by index's, data.
			/// </summary>
			virtual IData::ptr GetInputData( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Get an input, by index's, data.
			/// </summary>
			virtual IData::ptr GetInputData( size_t subSource, std::string name ) const = 0;

			/// <summary>
			/// Set an input, by index's, data.
			/// </summary>
			virtual void SetInputData( size_t subSource, size_t index, IData::ptr dataIn ) = 0;

			/// <summary>
			/// Adds an event: an action to be trigger by an input condition.
			/// </summary>
			virtual void AddEvent( unify::Owner::ptr owner, IInputCondition::ptr condition, IInputAction::ptr action ) = 0;

			/// <summary>
			/// Clears all input events.
			/// </summary>
			virtual void ClearEvents() = 0;

			/// <summary>
			/// Handles all events, returns the number of events that triggered.
			/// </summary>
			virtual int HandleEvents( float delta ) = 0;
		};
	}
}