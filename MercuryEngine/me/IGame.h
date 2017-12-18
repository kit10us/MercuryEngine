// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/Extension.h>
#include <me/input/InputManager.h>
#include <me/render/RenderInfo.h>
#include <me/render/RenderParams.h>
#include <me/action/IAction.h>
#include <me/object/action/IObjectAction.h>
#include <me/scene/IScene.h>
#include <me/scene/ISceneFactory.h>
#include <me/IGameComponent.h>
#include <me/UpdateParams.h>
#include <me/ICommandListener.h>
#include <rm/ResourceManagerSimple.h>
#include <rm/ResourceHub.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace me
{
	class ILogListener
	{
	public:
		~ILogListener() {}

		virtual void Log( std::string text ) = 0;
	};

	enum class ErrorLevel
	{
		Critical, // System is left unstable, we should likely shutdown immediately.
		Failure, // We have a failure, the module that reported it is likely corrupted.
		Warning // Error might not be important - state is unknown.
	};

	struct UpdateLock
	{
		typedef std::shared_ptr< UpdateLock > ptr;
	};

	/// <summary>
	/// A game, inheriting from this class, enables support of the necessary major game events.
	/// </summary>
	class IGame
	{
	protected:
		
		/// <summary>
		/// Setup is the initial event that is called to setup the game. It is the earliest point to instigate configuration.
		/// </summary>
		virtual bool Setup( IOS * os ) = 0;

		/// <summary>
		/// Startup is called to load assets and create game objects before Updating and Rendering. It is called once.
		/// </summary>
		virtual void Startup() = 0;

		/// <summary>
		/// Called once when engine is shutting down, to allow user to release assets that require manual release/destroy.
		/// </summary>
		virtual void Shutdown() = 0;

	public:
		virtual ~IGame() {}

		virtual OSParameters GetOSParameters() const = 0;

		/// <summary>
		/// Feed messages to game, and it's sub-systems.
		/// </summary>
		virtual void * Feed( std::string target, void * data ) = 0;

		virtual void Tick() = 0;

		virtual void Draw() = 0;

		virtual void SetOS( me::IOS::ptr os ) = 0;

		/// <summary>
		/// Get our OS interface.
		/// </summary>
		virtual IOS * GetOS() = 0;

		/// <summary>
		/// Get our RenderInfo.
		/// </summary>
		virtual const render::RenderInfo & GetRenderInfo() const = 0;

		virtual rm::ResourceHub & GetResourceHub() = 0;
		virtual const rm::ResourceHub & GetResourceHub() const = 0;

		virtual void Quit() = 0;

		virtual bool IsQuitting() const = 0;

		virtual unify::Owner::ptr GetOwnership() = 0;

		virtual input::InputManager * GetInputManager() = 0;
		virtual const input::InputManager * GetInputManager() const = 0;

		virtual void LogLine( std::string line, int indent = 2 ) = 0;
		virtual void AttachLogListener( ILogListener* listener ) = 0;
		virtual void DetachLogListener( ILogListener* litener ) = 0;

		/// <summary>
		/// This is our method of reporting issues, especially from modules.
		/// </summary>
		virtual void ReportError( ErrorLevel level, std::string source, std::string error ) = 0;

		virtual bool HadCriticalError() const = 0;

		virtual int GetComponentCount() const = 0;
		virtual void AddComponent( IGameComponent::ptr component ) = 0;
		virtual void RemoveComponent( IGameComponent::ptr component ) = 0;
		virtual IGameComponent::ptr GetComponent( int index ) = 0;
		virtual IGameComponent::ptr GetComponent( std::string name ) = 0;
		virtual int FindComponent( std::string name ) const = 0;

		/// <summary>
		/// Request access to safely update the game engine. This disables game updates.
		/// When exclusive, all other updates are prohibited.
		/// Fails if exclusive, and any other update locks are active.
		/// Returns a unique UpdateLock::ptr on success.
		/// Returns an empty UpdateLock::ptr on failure.			  
		/// Simply reset the UpdateLock::ptr, or let it fall out of scope, to unlock.
		/// </summary>
		virtual UpdateLock::ptr LockUpdate( bool exclusive ) = 0;

		/// <summary>
		/// If exclusive is false, returns true if there is any kind of lock,
		/// else it returns true only if there is an exclusive lock.
		/// </summary>
		virtual bool IsUpdateLocked( bool exclusive ) const = 0;

		/// <summary>
		/// Create an Action from an XML node.
		/// </summary>
		virtual action::IAction::ptr CreateAction(const qxml::Element * element) = 0;

		/// <summary>
		/// Create an Object Action from an XML node.
		/// </summary>
		virtual object::action::IObjectAction::ptr CreateObjectAction( const qxml::Element * element ) = 0;

		/// <summary>
		/// Create an Input Action from an XML node.
		/// </summary>
		virtual input::IInputAction::ptr CreateInputAction( const qxml::Element * element ) = 0;

		/// <summary>
		/// Add a command listener
		/// </summary>
		virtual void AddCommandListener( unify::Owner::weak_ptr owner, std::string command, ICommandListener::ptr listener ) = 0;

		/// <summmary>
		/// Fetch the ID for a corresponding command. If a command doesn't already exist, it's created.
		/// </summary>
		virtual size_t Command( std::string command ) = 0;

		/// <summmary>
		/// Fetch the name from an ID for a corresponding command. Returns an empty string if not found.
		/// </summary>
		virtual std::string Command( size_t id ) = 0;

		/// <summary>
		/// Send a command with extra information, and return a string result.
		/// Strings are used to best support cross DLL, and scripting support. 
		virtual std::string SendCommand( std::string command, std::string extra ) = 0;
			
		/// <summary>
		/// Send a command with extra information, and return a string result.
		/// Strings are used to best support cross DLL, and scripting support. 
		virtual std::string SendCommand( size_t id, std::string extra ) = 0;

		template< typename T >
		T* GetComponentT();
	};

	template< typename T >
	T* IGame::GetComponentT()
	{
		auto component = GetComponent( T::Name() );
		if( !component ) return nullptr;
		return dynamic_cast< T* >( component.get() );
	}
}
