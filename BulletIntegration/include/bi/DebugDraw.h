// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <bi/Convert.h>

namespace bi
{
	class DebugDraw : public btIDebugDraw
	{
	public:
		DebugDraw();
		void drawLine( const btVector3 & from, const btVector3 & to, const btVector3 & color );
		void drawContactPoint( const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color );
		void reportErrorWarning( const char * message );
		void draw3dText(const btVector3 & location,const char* textString);
		void setDebugMode(int debugMode);
		int	getDebugMode() const;
	private:
		int m_debugMode;
	};
} // using namespace bi