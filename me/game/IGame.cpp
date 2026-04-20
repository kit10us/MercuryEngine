// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/game/IGame.h>

#include <rm/IResource.h>
#include <io/ISource.h>
#include <me/script/IScript.h>
#include <me/render/ITexture.h>
#include <me/render/Effect.h>
#include <me/render/IVertexShader.h>
#include <me/render/IPixelShader.h>
#include <me/render/Geometry.h>

using namespace me;
using namespace me::render;
using namespace me::game;

template<>
rm::ResourceManager< io::ISource >* IGame::GetManager()
{
	auto rm = GetResourceHub().GetManager< io::ISource >("source");
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< io::ISource > * >(rm);
	return manager;
}

template<>
rm::ResourceManager< script::IScript >* IGame::GetManager()
{
	auto rm = GetResourceHub().GetManager< script::IScript >("script");
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< script::IScript > * >(rm);
	return manager;
}

template<>
rm::ResourceManager< ITexture >* IGame::GetManager()
{
	auto rm = GetResourceHub().GetManager< ITexture >("texture");
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< ITexture > * >(rm);
	return manager;
}

template<>
rm::ResourceManager< Effect >* IGame::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< Effect > * >(GetResourceHub().GetManager< Effect >("effect"));
	return manager;
}

template<>
rm::ResourceManager< IPixelShader >* IGame::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< IPixelShader > * >(GetResourceHub().GetManager< IPixelShader >("PixelShader"));
	return manager;
}

template<>
rm::ResourceManager< IVertexShader >* IGame::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< IVertexShader > * >(GetResourceHub().GetManager< IVertexShader >("VertexShader"));
	return manager;
}

template<>
rm::ResourceManager< Geometry >* IGame::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManager< Geometry > * >(GetResourceHub().GetManager< Geometry >("Geometry"));
	return manager;
}
