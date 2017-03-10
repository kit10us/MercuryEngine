// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/SceneViewer.h>
#include <meedr/ResourceBrowser.h>
#include <ui/Window.h>
#include <me/Geometry.h>

using namespace meedr;
					 
ResourceBrowser::ResourceBrowser( IWindow* parent, int nCmdShow, int x, int y, me::IGame * game )
	: Window( parent, L"ResourceBrowserWndClass" )
	, m_game{ game }
{
	using namespace create;
	AddContainer( new container::StackPanel( container::Stack::Vertical, 540, 440 ) );
	AddControl( new Static( L"Type:", 50, DefaultHeight() ) );
	AddControl( new Combobox( FillWidth(), DefaultHeight() ), "Types" );
	AddControl( new Listbox( FillWidth(), FillHeight() ), "Resources" );
	Create( L"Resource Browser", x, y, nCmdShow );
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
	Listbox* resources = GetControl< Listbox* >( "Resources" );

	// Clear contents...
	resources->ResetContent();

	size_t typeIndex = types->GetCurSel();

	// Fill in resource types...
	auto manager = m_game->GetResourceHub().GetManagerRaw( typeIndex );
	for ( size_t i = 0; i < manager->Count(); i++ )
	{			  
		std::string text = manager->GetResourceName( i );

		if ( unify::StringIs( manager->GetName(), "texture" ) )
		{
			auto textureManager = reinterpret_cast<rm::ResourceManagerSimple< me::ITexture >*>(manager);
			auto texture = textureManager->Get( i ).get();
			if ( !texture->GetParameters()->source.Empty() )
			{
				text += "  (" + texture->GetParameters()->source.ToString() + ")";
			}
		}	 
		else if ( unify::StringIs( manager->GetName(), "effect" ) )
		{
			auto effectManager = reinterpret_cast<rm::ResourceManagerSimple< me::Effect >*>(manager);
			auto effect = effectManager->Get( i ).get();
			if ( !effect->GetSource().empty() )
			{
				text += "  (" + effect->GetSource() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "vertexshader" ) )
		{
			auto vertexShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::IVertexShader >*>(manager);
			auto vertexShader = vertexShaderManager->Get( i ).get();
			if ( !vertexShader->GetSource().empty() )
			{
				text += "  (" + vertexShader->GetSource() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "pixelshader" ) )
		{
			auto pixelShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::IPixelShader >*>(manager);
			auto pixelShader = pixelShaderManager->Get( i ).get();
			if ( !pixelShader->GetSource().empty() )
			{
				text += "  (" + pixelShader->GetSource() + ")";
			}
		}
		else if ( unify::StringIs( manager->GetName(), "geometry" ) )
		{
			auto geometryManager = reinterpret_cast<rm::ResourceManagerSimple< me::Geometry >*>(manager);
			auto geometry = geometryManager->Get( i ).get();
			if ( !geometry->GetSource().empty() )
			{
				text += "  (" + geometry->GetSource() + ")";
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
	Listbox* resources = GetControl< Listbox* >( "Resources" );

	size_t typeIndex = types->GetCurSel();
	size_t i = resources->GetCurSel();

	auto manager = m_game->GetResourceHub().GetManagerRaw( typeIndex );
	unify::Path source;

	if ( unify::StringIs( manager->GetName(), "texture" ) )
	{
		auto textureManager = reinterpret_cast<rm::ResourceManagerSimple< me::ITexture >*>(manager);
		auto texture = textureManager->Get( i ).get();
		if ( !texture->GetParameters()->source.Empty() )
		{
			source = texture->GetParameters()->source;
		}
	}
	else if ( unify::StringIs( manager->GetName(), "effect" ) )
	{
		auto effectManager = reinterpret_cast<rm::ResourceManagerSimple< me::Effect >*>(manager);
		auto effect = effectManager->Get( i ).get();
		if ( !effect->GetSource().empty() )
		{
			source = effect->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "vertexshader" ) )
	{
		auto vertexShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::IVertexShader >*>(manager);
		auto vertexShader = vertexShaderManager->Get( i ).get();
		if ( !vertexShader->GetSource().empty() )
		{
			source = vertexShader->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "pixelshader" ) )
	{
		auto pixelShaderManager = reinterpret_cast<rm::ResourceManagerSimple< me::IPixelShader >*>(manager);
		auto pixelShader = pixelShaderManager->Get( i ).get();
		if ( !pixelShader->GetSource().empty() )
		{
			source = pixelShader->GetSource();
		}
	}
	else if ( unify::StringIs( manager->GetName(), "geometry" ) )
	{
		auto geometryManager = reinterpret_cast<rm::ResourceManagerSimple< me::Geometry >*>(manager);
		auto geometry = geometryManager->Get( i ).get();
		if ( !geometry->GetSource().empty() )
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

ui::IResult * ResourceBrowser::OnCreate( ui::Params params )
{
	using namespace ui;			  

	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnDestroy( ui::Params params )
{
	using namespace ui;			  

	GetParent()->SendUserMessage( RESOURCEBROWSER_CLOSED, Params{ 0, 0 } );
	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnAfterCreate( ui::Params )
{
	using namespace ui;			  

	UpdateResourceTypes();
	return new Result( 0 );
}

ui::IResult * ResourceBrowser::OnControlCommand( ui::ControlMessage message )
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
	else if ( message.IsFor( "Resources" ) )
	{
		switch ( message.code )
		{
		case LBN_DBLCLK:
		{
			OpenResource();
			break;
		}
		}
	}
	return new Result( 0 );
}			
