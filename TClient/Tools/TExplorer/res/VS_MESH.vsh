// VS_MESH.vsh
// Vertex shader for MESHVERTEX
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
//	r1 - Normal
//	r2 - Diffuse color


vs_2_x

dcl_position		v0	// Position input
dcl_normal			v1	// Normal input
dcl_texcoord		v2	// Texcoord input

//////////////////////////////////////////////////
// Compute position and normal
m4x3 r0.xyz, v0, c397
m3x3 r1.xyz, v1, c397

//////////////////////////////////////////////////
// Compute camera direction
add oT3.xyz, -r0, c1

//////////////////////////////////////////////////
// Compute position
mov r0.w, c0.x
m4x4 oPos, r0, c7

//////////////////////////////////////////////////
// Normalize normal
nrm r3, r1

mov oT5.xyz, c11
mov oT4.xyz, r3

//////////////////////////////////////////////////
// Compute diffuse color
mov r5.w, c0.y
mov r2, c2

loop aL, i0
	if_lt c11[aL].w, r5.w
		//////////////////////////////////////////////////
		// Point Light
		add r5.xyz, -r0.xyz, c11[aL].xyz
		dp3 r6.x, r5, r5
		mul r7.w, r6.x, c141[aL].x

		if_lt r7.w, c0.x
			lrp r6.xyz, r7.www, r5.www, c269[aL]
			nrm r7, r5
			dp3 r4.x, r3, -r7
			max r5.x, -r4.x, c0.y
			mul r4.xyz, r5.x, c140
			mul r4.xyz, r4, r6
			add r2.xyz, r2, r4
		endif
	else
		//////////////////////////////////////////////////
		// Directional Light
		dp3 r4.x, r3, c11[aL]
		max r5.x, -r4.x, c0.y
		mul r4.xyz, r5.x, c140
		mul r4.xyz, r4, c269[aL]
		mov r5, c139
		mad r4.xyz, r5, c141[aL], r4
		add r2.xyz, r2, r4
	endif
endloop

mov r2.w, c140.w
mov oD0, r2

//////////////////////////////////////////////////
// Compute texture coord
mov r3.xy, v2
mov r3.z, c0.x

m3x2 oT0.xy, r3, c3
m3x2 oT2.xy, r3, c5
