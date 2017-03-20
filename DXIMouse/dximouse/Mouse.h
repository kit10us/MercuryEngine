// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/InputSource.h>
#include <me/IGame.h>
#include <map>

namespace dximouse
{
	class Mouse : public me::input::InputSource
	{
	public:
		Mouse( me::IGame * game );
		virtual ~Mouse();

		std::string Name() const;

		void Update() override;

		size_t SubSourceCount() const override;

		size_t InputCount( size_t subSource ) const override;

		std::string InputName( size_t subSource, size_t index ) const override;

		size_t InputIndex( size_t subSource, std::string name ) const override;

		me::input::InputType GetInputType( size_t subSource, size_t index ) const override;

		me::input::IData::ptr GetInputData( size_t subSource, size_t index ) const override;

		void SetInputData( size_t subSource, size_t index, me::input::IData::ptr dataIn ) override;

	private:
		me::IGame * m_game;

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

		std::map< std::string, size_t, unify::CaseInsensitiveLessThanTest > m_nameToIndex;
		std::vector< Source > m_subSources;
		std::vector< Source > m_subSourcesUpdated;
	};
}