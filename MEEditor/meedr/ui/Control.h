// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/IControl.h>

namespace meedr
{
	namespace ui
	{
		class Control : public IControl
		{
			friend class Builder;
			friend class container::Canvas;
			friend class container::StackPanel;
		public:
			Control( int id );
			virtual ~Control();

			int GetID() override;

			int GetDefaultWidth() override;
			int GetDefaultHeight() override;

			void SetFillWidthWeight( int weight ) override;
			int GetFillWidthWeight() override;
			void SetFillHeightWeight( int weight ) override;
			int GetFillHeightWeight() override;

		protected:
			container::Container * GetParent() override;
			void SetParent( container::Container * parent ) override;

		private:
			container::Container * m_parent;
			int m_id;
			int m_fillWidthWeight;
			int m_fillHeightWeight;
		};
	}
}