// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <meedr/SceneViewer.h>
#include <meedr/ResourceBrowser.h>
#include <ui/Window.h>
#include <me/render/Geometry.h>

using namespace meedr;
					 
ResourceBrowser::ResourceBrowser( SceneViewer* parent, int nCmdShow, int x, int y, me::game::IGame * gameInstance )
	: Window( parent, L"ResourceBrowserWndClass" )
	, m_sceneViewer{ parent }
	, m_game{ gameInstance }
{
	using namespace create;
	AddContainer( new container::StackPanel( container::Stack::Vertical, 540, 440 ) );
	AddControl( new Static( L"Type:", 50, DefaultHeight() ) );
	AddControl( new Combobox( 230, DefaultHeight() ), "Types" );
	AddControl( new Listbox( 230, FillHeight() ), "Assets" );
	AddControl( new Button( L"Reload Asset", DefaultWidth(), DefaultHeight() ), "ReloadAsset" );
	Create( L"Asset Viewerr", x, y, nCmdShow );
}
									
void ResourceBrowser::UpdateResourceTypes()
{
	using namespace ui;			  

	Combobox* types = GetControl< Combobox* >( "Types" );

	// Clear contents...
	types->ResetContent();

	// Fill in resource types...
	for ( size_t i = 0; i < m_game->GetResourceHub().GetTypeCount(); i++ )
	{			  
		std::string name = m_game->GetResourceHub().GetTypeName( i ).c_str();
		types->AddString( name );
	}

	// Select first type:
	types->SetCurSel( 0 );

	UpdateResourceList();
}

void ResourceBrowser::UpdateResourceList()
{
	using namespace ui;			  

	Combobox* types = GetControl< Combobox* >( "Types" );
	Listbox* resources = GetControl< Listbox* >( "Assets" );

	// Clear contents...
	resources->ResetContent();

	size_t typeIndex = types->GetCurSel();

	// Fill in resource types...
	auto manager = m_game->GetResourceHub().GetManagerRaw( typeIndex );
	for ( size_t i = 0; i < manager->Count(); i++ )
	{			  
		std::string text = manager->GetResource( i )->GetName();

		if ( unify::StringIs( manager->GetName(), "texture" ) )
		{
			auto textureManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::ITexture >*>(manager);
			auto texture = textureManager->Get( i ).get();
			if ( !texture->GetParameters()->source.Empty() )
			{
				text += "  (" + texture->GetParameters()->source.ToString() + ")";
			}
		}	 
		else if ( unify::StringIs( manager->GetName(), "effect" ) )
		{
			auto effectManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::Effect >*>(manager);
			auto effect = effectManager->Get( i ).get();
			if ( !effect->GetSource().Empty() )
			{
				text += "  (" + effect->GetSource().ToString() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "vertexshader" ) )
		{
			auto vertexShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IVertexShader >*>(manager);
			auto vertexShader = vertexShaderManager->Get( i ).get();
			if ( !vertexShader->GetSource().Empty() )
			{
				text += "  (" + vertexShader->GetSource().ToString() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "pixelshader" ) )
		{
			auto pixelShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IPixelShader >*>(manager);
			auto pixelShader = pixelShaderManager->Get( i ).get();
			if ( !pixelShader->GetSource().Empty() )
			{
				text += "  (" + pixelShader->GetSource().ToString() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "geometry" ) )
		{
			auto geometryManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::Geometry >*>(manager);
			auto geometry = geometryManager->Get( i ).get();
			if ( !geometry->GetSource().Empty() )
			{
				text += "  (" + geometry->GetSource().ToString() + ")";
			}
		}

		resources->AddString( text.c_str() );
	}

	// Select first type:
	resources->SetCurSel( 0 );
}

void ResourceBrowser::OpenResource()
{
	using namespace ui;			  

	Combobox* types = GetControl< Combobox* >( "Types" );
	Listbox* resources = GetControl< Listbox* >( "Assets" );

	size_t typeIndex = types->GetCurSel();
	size_t i = resources->GetCurSel();

	auto manager = m_game->GetResourceHub().GetManagerRaw( typeIndex );
	unify::Path source;

	if ( unify::StringIs( manager->GetName(), "texture" ) )
	{
		auto textureManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::ITexture >*>(manager);
		auto texture = textureManager->Get( i ).get();
		if ( !texture->GetParameters()->source.Empty() )
		{
			source = texture->GetParameters()->source;
		}
	}
	else if ( unify::StringIs( manager->GetName(), "effect" ) )
	{
		auto effectManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::Effect >*>(manager);
		auto effect = effectManager->Get( i ).get();
		if ( !effect->GetSource().Empty() )
		{
			source = effect->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "vertexshader" ) )
	{
		auto vertexShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IVertexShader >*>(manager);
		auto vertexShader = vertexShaderManager->Get( i ).get();
		if ( !vertexShader->GetSource().Empty() )
		{
			source = vertexShader->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "pixelshader" ) )
	{
		auto pixelShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IPixelShader >*>(manager);
		auto pixelShader = pixelShaderManager->Get( i ).get();
		if ( !pixelShader->GetSource().Empty() )
		{
			source = pixelShader->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "geometry" ) )
	{
		auto geometryManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::Geometry >*>(manager);
		auto geometry = geometryManager->Get( i ).get();
		if ( !geometry->GetSource().Empty() )
		{
			source = geometry->GetSource();
		}
	}

	if ( source.Exists() )
	{
		char operation[] = "open";
		source = m_game->GetOS()->GetRunPath() + source ;
		HINSTANCE result = ShellExecuteA( GetHandle(), operation, source.ToString().c_str(), nullptr, nullptr, SW_SHOW );
		switch( (int)result )
		{
		case ERROR_FILE_NOT_FOUND:
		{
			break;
		}
		}
	}
}

void ResourceBrowser::ReloadResource()
{
	using namespace ui;

	Combobox* types = GetControl< Combobox* >( "Types" );
	Listbox* resources = GetControl< Listbox* >( "Assets" );

	size_t typeIndex = types->GetCurSel();
	size_t i = resources->GetCurSel();

	auto manager = m_game->GetResourceHub().GetManagerRaw( typeIndex );
	unify::Path source;

	if( unify::StringIs( manager->GetName(), "texture" ) )
	{
		auto textureManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::ITexture >*>( manager );
		auto texture = textureManager->Get( i ).get();
		texture->Reload();
	}
	else if( unify::StringIs( manager->GetName(), "vertexshader" ) )
	{
		auto vertexShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IVertexShader >*>( manager );
		auto vertexShader = vertexShaderManager->Get( i ).get();
		vertexShader->Reload();
	}
	else if( unify::StringIs( manager->GetName(), "pixelshader" ) )
	{
		auto pixelShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::IPixelShader >*>( manager );
		auto pixelShader = pixelShaderManager->Get( i ).get();
		pixelShader->Reload();
	}
	else if( unify::StringIs( manager->GetName(), "geometry" ) )
	{
		auto geometryManager = reinterpret_cast<rm::ResourceManagerSimple< me::render::Geometry >*>( manager );
		auto geometry = geometryManager->Get( i ).get();
		geometry->Reload();
	}
}

ui::IResult * ResourceBrowser::OnCreate( ui::message::Params params )
{
	using namespace ui;			  

	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnDestroy( ui::message::Params params )
{
	using namespace ui;			  

	GetParent()->SendUserMessage( RESOURCEBROWSER_CLOSED, message::Params{ 0, 0 } );
	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnAfterCreate( ui::message::Params )
{
	using namespace ui;			  

	UpdateResourceTypes();
	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnControlCommand( ui::message::ControlCommand message )
{
	using namespace ui;			  

	if ( message.IsFor( "Types" ) )
	{
		switch ( message.code )
		{
		case CBN_SELCHANGE:
		{
			UpdateResourceList();
			break;
		}
		}
	}	  
	else if ( message.IsFor( "Assets" ) )
	{
		switch ( message.code )
		{
		case LBN_DBLCLK:
			OpenResource();
			break;
		}
	}
	else if( message.IsFor( "ReloadAsset" ) )
	{
		switch( (Button::Event)message.code )
		{
		case Button::Event::Clicked:
			ReloadResource();
			break;
		}
	}
	return new Result( 0 );
}			
