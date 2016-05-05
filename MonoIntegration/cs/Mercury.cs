using System;
using System.Runtime.InteropServices;

namespace Mercury
{
	public class Mercury
	{
	}
	
	public class Debug
	{
		[DllImport ("__Internal", EntryPoint="CSMercury_Debug_Output")]
		public static extern void Output( String text );
	}
	
	public class Configuration
	{
		[DllImport ("__Internal", EntryPoint="CSMercury_Configuration_SetResolution")]
		public static extern void SetResolution( uint width, uint height );

		[DllImport ("__Internal", EntryPoint="CSMercury_Configuration_SetFullscreen")]
		public static extern void SetFullscreen( bool fullscreen );
	}
	
	namespace Setup
	{
		public class Configuration
		{
			
		}
	}
	
	public interface IGame
	{
		void Setup( Setup.Configuration configuration );
		void Startup();
		bool Update( float elapsed );
		void Render();
		void Shutdown();
	}
}