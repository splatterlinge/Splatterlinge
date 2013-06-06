#version 120

varying vec3 vVertex;

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;


void main()
{
	vec3 reflection = vec3( texture2DProj( reflectionMap, gl_TexCoord[0] ) );
	vec3 refraction = vec3( texture2DProj( refractionMap, gl_TexCoord[0] ) );
	vec3 finalColor = mix( reflection, refraction, 0.5 );
	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );
	gl_FragColor = vec4( finalFragment, 1 );
}
