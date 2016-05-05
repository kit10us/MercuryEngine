using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class True : Value
	{
		public bool value { get { return true; } }

		public True()
			: base( ValueType.True )
		{
		}

		public True( StringBuilder text )
			: base( ValueType.True )
		{
			ConsumeWhiteSpaces( text );

			if ( BeginsWith( text, "true" ) == false )
			{
				throw new Exception( "JSON True not well formed!" );
			}
			text.Remove( 0, 4 );
		}

		public override string ToString()
		{
			return "true";
		}

		public override string ToJSONString()
		{
			return ToString();
		}

		public override bool ToBool()
		{
			return true;
		}
	}
}
