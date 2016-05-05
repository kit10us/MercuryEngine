using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

/// See http://json.org/ for implementation details.
namespace Amd.JSON
{
	public class Array : Value, IEnumerable< Value >
	{
		public List<Value> elements = new List<Value>();

		public Array()
			: base( ValueType.Array )
		{
		}

		public Array( Value firstValue )
			: base( ValueType.Array )
		{
			elements.Add( firstValue );
		}

		public Array( List<Value> values )
			: base( ValueType.Array )
		{
			elements.AddRange( values );
		}

		public Array( Value [] values )
			: base( ValueType.Array )
		{
			elements.AddRange( values );
		}

		public Array( StringBuilder text )
			: base( ValueType.Array )
		{
			if ( text.Length == 0 || text[ 0 ] != '[' )
			{
				throw new Exception( "JSON Array needs to beging with '['!" );
			}

			text.Remove( 0, 1 );

			// Parse values...
			while ( text.Length > 0 && text[ 0 ] != ']' )
			{
				elements.Add( Value.MakeValue( text ) );
				if ( text[ 0 ] == ',' )
				{
					text.Remove( 0, 1 );
				}
			};

			if ( text[ 0 ] != ']' )
			{
				throw new Exception( "JSON Array needs to end with ']'!" );
			}
			text.Remove( 0, 1 );
		}

		public Array( XmlNode node )
			: base( ValueType.Array )
		{
			XmlNodeList items = node.SelectNodes( "./item" );
			foreach( XmlNode item in items )
			{
				Add( MakeValue( item ) );
			}
		}

		public override string ToString()
		{
			string output = "[";
			elements.ForEach( x =>
			{
				if ( output.Length > 1 ) output += ",";
				output += x.ToJSONString();
			} );
			output += "]";
			return output;
		}

		public Amd.JSON.Value Add( Value value )
		{
			elements.Add( value );
			return value;
		}

		public Value this[ int index ]
		{
			get
			{
				return elements[ index ];
			}
		}

		public override string ToJSONString()
		{
			return ToString();
		}

		public override bool IsNullOrEmpty()
		{
			return elements.Count == 0;
		}

		public IEnumerator<Amd.JSON.Value> GetEnumerator()
		{
			return elements.GetEnumerator();
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			return GetEnumerator();
		}
	}
}
