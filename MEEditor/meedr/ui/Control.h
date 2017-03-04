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
			friend class Window;
			friend class container::Canvas;
			friend class container::StackPanel;

		protected:
			Control( int width, int height );

		public:
			virtual ~Control();

			// Default values...
			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

			// Wanted values...
			void SetFillWidthWeight( int weight ) override;
			int GetFillWidthWeight() const override;

			void SetFillHeightWeight( int weight ) override;
			int GetFillHeightWeight() const override;

			int GetWantedWidth() const override;
			int GetWantedHeight() const override;

			std::string GetName() const override;

			// Actual values...
			int GetActualX() const override;
			int GetActualY() const override;
			int GetActualWidth() const override;
			int GetActualHeight() const override;

			void SetID( int id ) override;
			int GetID() const override;

		protected:
			container::Container * GetParent() const override;
			void SetParent( container::Container * parent ) override;

			void ComputePass1() override;
			void ComputePass2( int fillWidthTotal, int fillHeightTotal, int fillWidthTotalWeight, int fillHeightTotalWeight ) override;
			void ComputePass3( int x, int y ) override;

			container::Container * m_parent;
			int m_id;
			std::string m_name;
			int m_fillWidthWeight;
			int m_fillHeightWeight;
			int m_wantedWidth;
			int m_wantedHeight;
			int m_actualX;
			int m_actualY;
			int m_actualWidth;
			int m_actualHeight;
		};
	}
}