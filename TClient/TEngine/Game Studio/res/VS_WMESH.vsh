// VS_WMESH.vsh
// Vertex shader for WMESHVERTEX
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
//	r0 - Weight
//	r1 - Weight indices
//	r2 - Position
//	r3 - Normal
//	r4 - Diffuse color


vs_2_x

dcl_position		v0	// Position input
dcl_blendweight		v1	// Weight input
dcl_blendindices	v2	// Blend indices input
dcl_normal			v3	// Normal input
dcl_texcoord		v4	// Texcoord input (Stage 1)

if b0
	//////////////////////////////////////////////////
	// D3DCOLORtoUBYTE4 operation
	mul r1, v2.zyxw, c0.wwww

	//////////////////////////////////////////////////
	// Compute the last blending weight
	mov r0.xyz, v1.xyz
	dp3 r0.w, v1.xyz, c0.xxx	// Sum of weight
	add r0.w, -r0.w, c0.x		// 1.0 - Sum of weight

	//////////////////////////////////////////////////
	// Compute position and normal
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// 1st index
	mova a0.x, r1.x
	m4x3 r2.xyz, v0, c397[a0.x]
	m3x3 r3.xyz, v3, c397[a0.x]

	mul r2, r2, r0.xxxx
	mul r3, r3, r0.xxxx

	//////////////////////////////////////////////////
	// 2nd index
	mova a0.x, r1.y
	m4x3 r4.xyz, v0, c397[a0.x]
	m3x3 r5.xyz, v3, c397[a0.x]

	mad r2, r4, r0.yyyy, r2
	mad r3, r5, r0.yyyy, r3

	//////////////////////////////////////////////////
	// 3rd index
	mova a0.x, r1.z
	m4x3 r4.xyz, v0, c397[a0.x]
	m3x3 r5.xyz, v3, c397[a0.x]

	mad r2, r4, r0.zzzz, r2
	mad r3, r5, r0.zzzz, r3

	//////////////////////////////////////////////////
	// 4th index
	mova a0.x, r1.w
	m4x3 r4.xyz, v0, c397[a0.x]
	m3x3 r5.xyz, v3, c397[a0.x]

	mad r2, r4, r0.wwww, r2
	mad r3, r5, r0.wwww, r3
else
	//////////////////////////////////////////////////
	// Compute position and normal
	m4x3 r2.xyz, v0, c397
	m3x3 r3.xyz, v3, c397
endif

//////////////////////////////////////////////////
// Compute camera direction
add oT3.xyz, -r2, c1

//////////////////////////////////////////////////
// Compute position
mov r2.w, c0.x
m4x4 oPos, r2, c7

//////////////////////////////////////////////////
// Normalize normal
nrm r5, r3

mov oT5.xyz, c11
mov oT4.xyz, r5

//////////////////////////////////////////////////
// Compute diffuse color
mov r1.w, c0.y
mov r4, c2

loop aL, i0
	if_lt c11[aL].w, r1.w
		//////////////////////////////////////////////////
		// Point Light
		add r1.xyz, -r2.xyz, c11[aL].xyz
		dp3 r3.x, r1, r1
		mul r7.w, r3.x, c141[aL].x

		if_lt r7.w, c0.x
			lrp r3.xyz, r7.www, r1.www, c269[aL]
			nrm r7, r1
			dp3 r6.x, r5, -r7
			max r7.x, -r6.x, c0.y
			mul r6.xyz, r7.x, c140
			mul r6.xyz, r6, r3
			add r4.xyz, r4, r6
		endif
	else
		//////////////////////////////////////////////////
		// Directional Light
		dp3 r6.x, r5, c11[aL]
		max r7.x, -r6.x, c0.y
		mul r6.xyz, r7.x, c140
		mul r6.xyz, r6, c269[aL]
		mov r7, c139
		mad r6.xyz, r7, c141[aL], r6
		add r4.xyz, r4, r6
	endif
endloop

mov r4.w, c140.w
mov oD0, r4

//////////////////////////////////////////////////
// Compute texture coord
mov r5.xy, v4
mov r5.z, c0.x

m3x2 oT0.xy, r5, c3
m3x2 oT2.xy, r5, c5
