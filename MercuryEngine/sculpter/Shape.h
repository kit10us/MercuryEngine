// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <sculpter/Sheet.h>
#include <vector>
#include <memory>

namespace sculpter
{
	class Shape
	{
	public:
		typedef std::shared_ptr< Shape > ptr;

		Shape();
		~Shape();

		void AddSheet( Sheet::ptr sheet );
		size_t GetSheetCount() const;

	private:
		std::vector< Sheet::ptr > m_sheets;
	};
}