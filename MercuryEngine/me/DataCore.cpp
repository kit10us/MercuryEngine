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

void DataCore::OnBeforeStartup( IGame * game )
{
}

void DataCore::OnAfterStartup( IGame * game )
{
}

void DataCore::OnUpdate( IGame * game, UpdateParams params )
{
}

void DataCore::OnRender( IGame * game, RenderParams params )
{
}

void DataCore::OnDetach( IGame * game )
{
}
