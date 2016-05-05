using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class False : Value
	{
		public bool value { get { return false; } }

		public False()
			: base( ValueType.False )
		{
		}

		public False( StringBuilder text )
			: base( ValueType.False )
		{
			ConsumeWhiteSpaces( text );

			if ( BeginsWith( text, "false" ) == false )
			{
				throw new Exception( "JSON False not well formed!" );
			}
			text.Remove( 0, 5 );
		}

		public override string ToString()
		{
			return "false";
		}

		public override string ToJSONString()
		{
			return ToString();
		}

		public override bool ToBool()
		{
			return false;
		}
	}
}
