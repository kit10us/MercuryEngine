using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Number : Value
	{
		public double value { get; set; }

		/// <summary>
		/// Creates a number from text. Will modifiy the text, just leaving the left over bits.
		/// </summary>
		public Number()
			: base( ValueType.Number )
		{
		}

		public Number( StringBuilder text )
			: base( ValueType.Number )
		{
			ConsumeWhiteSpaces( text );

			int i = 0;
			for ( i = 0; i < text.Length && ( ( text[ i ] >= '0' && text[ i ] <= '9' ) || text[ i ] == '-' || text[ i ] == '+' || text[ i ] == 'e' || text[ i ] == 'E' || text[ i ] == '.' ); ++i )
			{
			}
			char[] temp = new char[ i ];
			text.CopyTo( 0, temp, 0, i );
			value = double.Parse( new string( temp ) );
			text.Remove( 0, i );
		}

		public Number( double value )
			: base( ValueType.Number )
		{
			this.value = value;
		}

		public override string ToString()
		{
			return value.ToString();
		}

		public override string ToJSONString()
		{
			return ToString();
		}

		public override double ToDouble()
		{
			return value;
		}
	}
}
