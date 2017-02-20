// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

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
		class IInputSource
		{
		public:
			typedef std::shared_ptr< IInputSource > ptr;

			virtual ~IInputSource() {}

			/// <summary>
			/// Name of our source.
			/// </summaty>
			virtual std::string Name() const = 0;

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
			virtual size_t InputCount( size_t subSource ) const = 0;

			/// <summary>
			/// Get the name for an input by index.
			/// </summary>
			virtual std::string InputName( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Get hte index of an input by name.
			/// </summary>
			virtual size_t InputIndex( size_t subSource, std::string name ) const = 0;

			/// <summary>
			/// Number of conditions for an input.
			/// </summary>
			virtual size_t InputConditionCount( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Number of inputs available from this source.
			/// </summary>
			virtual std::string InputConditionName( size_t subSource, size_t index, size_t condition ) const = 0;

			/// <summary>
			/// Number of inputs available from this source.
			/// </summary>
			virtual size_t InputConditionIndex( size_t subSource, size_t index, std::string condition ) const = 0;

			/// <summary>
			/// Returns the state of an input by name, for a specific condition.
			/// </summary>
			/// <exmaple>
			/// if ( State::True == GetState( 0, "Start", "Pressed" ) ) { ... }
			/// </example>
			virtual State GetState( size_t subSource, size_t index, size_t condition ) const = 0;

			/// <summary>
			/// Returns true if named input can have a value (in other words, does "name" actual exist as a value).
			/// </summary>
			virtual bool HasValue( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Returns value of named input; use HasValue first to verify the named values exists (once true, will always be true).
			/// If the named input doesn't exist, this will return 0.
			/// Valid ranges are 0.0 to 1.0f
			/// </summary>
			virtual float GetValue( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Sets a writeable state. Returns false if we can't write to the state.
			/// </summary>
			virtual bool SetState( size_t subSource, size_t index, size_t condition, bool set ) = 0;

			/// <summary>
			/// Sets a writeable value. Returns false if we can't write to the value.
			/// Valid ranges are 0.0 to 1.0f
			/// </summary>
			virtual bool SetValue( size_t subSource, size_t index, float value ) = 0;
		};
	}
}