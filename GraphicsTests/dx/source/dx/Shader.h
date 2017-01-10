#pragma once

#include <dx/Common.h>

namespace dx
{
	class Shader
	{
	public:
		virtual ~Shader() {};
		virtual void Use() = 0;
	};
}