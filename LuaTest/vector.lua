function InsideV3( x, y, z )
	return { x=x, y=y, z=z}
end

v = InsideV3( 1, 2, 3 )
print( "V3 created inside LUA..." )
print( v )
print( v.x )
print( v.y )
print( v.z )
print()

v2 = V3.New( 3, 4, 5 )
print( "V3 created from C++..." )
print( v2 )
print( v2.x )
print( v2.y )
print( v2.z )
print( V3.ToString( v ) )
print( V3.ToString( v2 ) )
print()
