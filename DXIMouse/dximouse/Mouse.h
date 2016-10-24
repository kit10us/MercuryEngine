// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/input/IInputSource.h>
#include <dxi/core/IGame.h>
#include <map>

namespace dximouse
{
	class Mouse : public dxi::input::IInputSource
	{
	public:
		Mouse( dxi::core::IGame * game );
		virtual ~Mouse();

		std::string Name() const;

		void Update() override;

		size_t SubSourceCount() const override;

		dxi::input::State GetState( size_t subSource, std::string name, std::string condition ) const override;

		bool HasValue( size_t subSource, std::string name ) const override;

		float GetValue( size_t subSource, std::string name ) const override;

		bool SetState( size_t subSource, std::string name, std::string condition, bool set );

		bool SetValue( size_t subSource, std::string name, float value );

	private:
		dxi::core::IGame * m_game;

		struct Source
		{
			Source()
				: LeftButton( false )
				, LeftButtonPressed( false )
				, RightButton( false )
				, RightButtonPressed( false )
				, MiddleButton( false )
				, MiddleButtonPressed( false )
				, PositionX( -1.0f )
				, PositionY( -1.0f )
				, ChangeX( 0.0f )
				, ChangeY( 0.0f )
				, MouseWheel( 0.0f )
			{
			}

			bool LeftButton;
			bool LeftButtonPressed;
			bool RightButton;
			bool RightButtonPressed;
			bool MiddleButton;
			bool MiddleButtonPressed;

			float PositionX;
			float ChangeX;
			float PositionY;
			float ChangeY;
			float MouseWheel;
		};

		std::vector< Source > m_subSources;
		std::vector< Source > m_subSourcesUpdated;
	};
}