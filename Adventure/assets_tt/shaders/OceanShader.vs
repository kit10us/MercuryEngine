vs.1.1
dcl_position v0
dcl_normal v1
dcl_texcoord0 v2

def c12, 10, 10, 10, 1
def c14, 1, 1, 1, 1	

// Transform position
//m4x4 oPos, v0, c4
mul r0, v0.x, c4
mad r0, v0.y, c5, r0
mad r0, v0.z, c6, r0
add oPos, c7, r0

// Transform normal
dp3 r1.x, v1, c0
dp3 r1.y, v1, c1
dp3 r1.z, v1, c2

// Re-normalize
dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w


//mul oPos, oPos, c12

// Transform normal
//m4x4 r3, v3, c0

mov oD0, c14
mov oT0.xy, v2
