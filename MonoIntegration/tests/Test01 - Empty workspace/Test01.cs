using System;
using Mercury;

public class MyGame// : Mercury.IGame
{
	public bool Setup() // TODO: use return value in engine.
	{
		Mercury.Debug.Output( "\nSetup sucessfully called!\n" );
		Mercury.Configuration.SetResolution( 800, 600 );
		Mercury.Configuration.SetFullscreen( false );
		return false;
	}
	
	public void Startup()
	{
	}
	
	public bool Update( float elapsed )
	{
		return true;
	}
	
	public void Render()
	{
	}
	
	public void Shutdown()
	{
	}
}
