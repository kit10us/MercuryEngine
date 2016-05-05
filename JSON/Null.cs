using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Null : Value
	{
		public string value { get { return null; } }

		public Null()
			: base( ValueType.Null )
		{
		}

		public Null( StringBuilder text )
			: base( ValueType.Null )
		{
			ConsumeWhiteSpaces( text );

			if ( BeginsWith( text, "null" ) == false )
			{
				throw new Exception( "JSON Null not well formed!" );
			}
			text.Remove( 0, 4 );
		}

		public override string ToString()
		{
			return "null";
		}

		public override string ToJSONString()
		{
			return ToString();
		}

		public override bool IsNullOrEmpty()
		{
			return true;
		}
	}
}
