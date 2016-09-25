using System;
using System.Runtime.InteropServices;

public class HelloWorld
{
	[DllImport ("__Internal", EntryPoint="DoSomething")]
	static extern void DoSomething();

	static public void Main()
	{
		try {
			DoSomething();
		} catch
		{
			Console.WriteLine( "Failed to call \"DoSomething\", likely running .exe outside of Mono Test!" );
		}
	}
}