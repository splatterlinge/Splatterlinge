// based upon OculusSDK Sample

uniform vec2 lensCenter;
uniform vec2 scale;
uniform vec2 scaleIn;
uniform vec4 hmdWarpParam;
uniform vec4 chromAbParam;
uniform sampler2D sourceMap;

// scales input texture coordinates for distortion.
// scaleIn maps texture coordinates to scales to ([-1, 1]), although top/bottom will be
// larger due to aspect ratio.
void main()
{
	vec2  theta = ( gl_TexCoord[0].st - lensCenter ) * scaleIn;	// scales to [-1, 1]
	float rSq = theta.x * theta.x + theta.y * theta.y;
	vec2  theta1 = theta * (
		hmdWarpParam.x +
		hmdWarpParam.y * rSq +
		hmdWarpParam.z * rSq * rSq +
		hmdWarpParam.w * rSq * rSq * rSq );

	// Detect whether blue texture coordinates are out of range since these will scaled out the furthest.
	vec2 thetaBlue = theta1 * ( chromAbParam.z + chromAbParam.w * rSq );
	vec2 tcBlue = lensCenter + scale * thetaBlue;
	if( !all(equal( clamp( tcBlue, vec2( 0.0, 0.0 ), vec2( 1.0, 1.0 ) ), tcBlue )) )
	{
		gl_FragColor = vec4( 0 );
		return;
	}

	// Now do blue texture lookup.
	float blue = texture2D( sourceMap, tcBlue ).b;

	// Do green lookup (no scaling).
	vec2  tcGreen = lensCenter + scale * theta1;
	vec4  center = texture2D( sourceMap, tcGreen );

	// Do red scale and lookup.
	vec2  thetaRed = theta1 * ( chromAbParam.x + chromAbParam.y * rSq );
	vec2  tcRed = lensCenter + scale * thetaRed;
	float red = texture2D( sourceMap, tcRed ).r;

	gl_FragColor = vec4( red, center.g, blue, center.a );
}
