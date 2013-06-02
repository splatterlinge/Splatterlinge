#version 120

varying vec3 vVertex;
varying vec3 vViewDir;
varying vec3 vSunDir;

uniform float cloudiness;
uniform float smoothness;
uniform float horizonFade;

uniform sampler2D cloudMap;


void main()
{
	vec3 viewDir = normalize( vViewDir );
	vec3 sunDir = normalize( vSunDir );

	float depth = cloudiness;
	depth += texture2D( cloudMap, gl_TexCoord[0].xy ).r;
	depth -= texture2D( cloudMap, gl_TexCoord[1].xy ).r;
	depth = max( 0.0, depth );

	float density = pow( depth, smoothness );
	float cloudAlpha = clamp( density, 0.0, 1.0 ) * clamp( viewDir.y*horizonFade, 0.0, 1.0 );
	vec3 finalColor = gl_LightSource[0].ambient.xyz + gl_LightSource[0].diffuse.xyz;

	gl_FragColor = vec4( finalColor * cloudAlpha, cloudAlpha );

//	float sunFactor = pow( max( 0.0, dot( normalize(vVertex), sunDir ) ), 3.0 );
//	float scatter = max( 0.0, sunFactor - density ) * clamp( viewDir.y*horizonFade, 0.0, 1.0 );
//	gl_FragColor = vec4( finalColor * cloudAlpha + scatter, cloudAlpha );
//	gl_FragColor = vec4( scatter, scatter, scatter, cloudAlpha );
}
