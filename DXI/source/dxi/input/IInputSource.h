// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>
#include <string>

namespace dxi
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
		/// A source of input.
		/// </summary>
		class IInputSource
		{
		public:
			typedef std::shared_ptr< IInputSource > ptr;
			
			virtual ~IInputSource(){}

			/// <summary>
			/// Name of our source.
			/// </summaty>
			virtual std::string Name() const = 0;

			/// <summary>
			/// Update called once a game cycle to enable us to update and poll our inputs.
			/// </summary>
			virtual void Update() = 0;

			/// <summary>
			/// Number of sub-sources, since we are currently only expected to have one instance of an input source, this allows us to have
			/// multiple of the input sources.
			/// </summary>
			virtual size_t SubSourceCount() const = 0;

			/// <summary>
			/// Returns the state of an input by name, for a specific condition.
			/// </summary>
			/// <exmaple>
			/// if ( State::True == GetState( 0, "Start", "Pressed" ) ) { ... }
			/// </example>
			virtual State GetState( size_t subSource, std::string name, std::string condition ) const = 0;

			/// <summary>
			/// Returns true if named input can have a value (in other words, does "name" actual exist as a value).
			/// </summary>
			virtual bool HasValue( size_t subSource, std::string name ) const = 0;

			/// <summary>
			/// Returns value of named input; use HasValue first to verify the named values exists (once true, will always be true).
			/// If the named input doesn't exist, this will return 0.
			/// Valid ranges are 0.0 to 1.0f
			/// </summary>
			virtual float GetValue( size_t subSource, std::string name ) const = 0;	

			/// <summary>
			/// Sets a writeable state. Returns false if we can't write to the state.
			/// </summary>
			virtual bool SetState( size_t subSource, std::string name, std::string condition, bool set ) = 0;

			/// <summary>
			/// Sets a writeable value. Returns false if we can't write to the value.
			/// Valid ranges are 0.0 to 1.0f
			/// </summary>
			virtual bool SetValue( size_t subSource, std::string name, float value ) = 0;
		};
	}
}