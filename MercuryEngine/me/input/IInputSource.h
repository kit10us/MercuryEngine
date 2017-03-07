// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputType.h>
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
			/// Get the index of an input by name.
			/// </summary>
			virtual size_t InputIndex( size_t subSource, std::string name ) const = 0;

			/// <summary>
			/// Returns the type of an input, by index.
			/// </summary>
			virtual InputType GetInputType( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Get an input, by index's, data.
			/// </summary>
			virtual IData::ptr GetInputData( size_t subSource, size_t index ) const = 0;

			/// <summary>
			/// Set an input, by index's, data.
			/// </summary>
			virtual void SetInputData( size_t subSource, size_t index, IData::ptr dataIn ) = 0;
		};
	}
}