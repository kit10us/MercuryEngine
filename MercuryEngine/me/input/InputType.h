// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <unify/V3.h>
#include <memory>
#include <string>

namespace me
{
	namespace input
	{
		/// <summary>
		/// 
		/// </summary>
		enum class InputType
		{
			Invalid = -1,
			Button,
			Toggle,
			Tracker,
			Trigger,
			Motion,
			Stick,
		};
		std::string InputTypeToString( InputType type );
		InputType StringToInputType(std::string type);

		struct IData
		{
			typedef std::shared_ptr< IData > ptr;

			IData( InputType type ) : type{ type } {}
			virtual ~IData() {}
			
			InputType type;
		};

		struct ButtonData : public IData
		{				   
			ButtonData() : IData{ InputType::Button }, down{ false }, pressed{ false } { }
			bool down;
			bool pressed;
		};

		struct ToggleData : public IData
		{
			ToggleData() : IData{ InputType::Toggle }, on{ false } { }
			bool on;
		};

		struct TrackerData : public IData
		{
			TrackerData() : IData{ InputType::Tracker }, position{ 0, 0, 0 }, change{ 0, 0, 0 }, set{ false, false, false } { }
			unify::V3< float > position;
			unify::V3< float > change;
			unify::V3< bool > set;
		};

		struct TriggerData : public IData
		{
			TriggerData() : IData{ InputType::Trigger }, value{ 0 } { }
			float value;
		};

		struct MotionData : public IData
		{
			MotionData() : IData{ InputType::Motion } { }
		};

		struct StickData : public IData
		{
			StickData() : IData{ InputType::Stick }, axis{ 0, 0, 0 } { }
			unify::V3< float > axis;
		};
	}
}