// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/DataCore.h>

using namespace me;

DataCore::DataCore()
{
}

DataCore::~DataCore()
{
}

std::string DataCore::GetName() const
{
	return "DataCore";
}

void DataCore::OnAttach( IGame * game )
{
}

void DataCore::OnUpdate( IGame * game, IRenderer * renderer, const RenderInfo & renderInfo )
{
}

void DataCore::OnRender( IGame * game, IRenderer * renderer, const RenderInfo & renderInfo )
{
}

void DataCore::OnDetach( IGame * game )
{
}
