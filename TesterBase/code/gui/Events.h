#pragma once

namespace gui
{
	class Thing;

	struct OnEventFunctor
	{
		virtual ~OnEventFunctor() {}
		virtual void operator()( Thing & thing ) = 0;
	};
}