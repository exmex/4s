// VS_LVERTEX.vsh
// Vertex shader for LVERTEX
//
// Registers:
//
//	Reg		Size	Comment
//	------- ------- ---------------------------------------
//	b0		1		Skinning flag
//	i0		1		Light count
//	c0		1		1.0, 0.0 and D3DCOLORtoUBYTE4 constant
//	c1		1		Camera position
//	c2		1		Ambient
//	c3		2		Tex matrix for stage 0
//	c5		2		Tex matrix for stage 1
//	c7		4		Proj matrix
//	c11		128		Light direction
//	c139	1		Mtrl ambient
//	c140	1		Mtrl diffuse
//	c141	128		Light ambient
//	c269	128		Light diffuse
//	c397	512		World matrix
//
//
// Default values:
//
//	c0 = { 1.0, 0.0, 0.0, 765.005859};	// 1.0, 0.0 and D3DCOLORtoUBYTE4 constant
//	b0 = true;							// Skinning flag
//	i0 = { 128, 0, 1, 0};				// Light count
//
//
// Temporary Register:
//
//	r0 - Position


vs_2_x

dcl_position		v0	// Position input
dcl_color			v1	// Diffuse input
dcl_texcoord		v2	// Texcoord input

//////////////////////////////////////////////////
// Compute position
m4x3 r0.xyz, v0, c397

//////////////////////////////////////////////////
// Compute camera direction and Light direction
add r2.xyz, -r0, c1
mov oT3.xyz, r2
mov oT5.xyz, c11

//////////////////////////////////////////////////
// Compute oT4
dp3 r2.w, r2, r2
rsq r2.x, r2.w
mul r2.x, r2.x, c2.x
rcp r2.x, r2.x
add r2.y, -r2.x, c2.y
max r2.y, r2.y, c0.y
mov r2.z, c0.y
mov oT4.xyz, r2

//////////////////////////////////////////////////
// Compute position
mov r0.w, c0.x
m4x4 oPos, r0, c7

//////////////////////////////////////////////////
// Compute diffuse color
mov oD0, v1

//////////////////////////////////////////////////
// Compute texture coord
mov r1.xy, v2
mov r1.z, c0.x

m3x2 oT0.xy, r1, c3
m3x2 oT2.xy, r1, c5
