#version 120

varying vec3 vVertex;

uniform sampler2D directMap;


void main()
{
	vec3 viewDir = normalize( -vVertex );
	vec4 finalColor = texture2D( directMap, gl_TexCoord[0].st ) * gl_Color;

	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor.rgb, fogFactor );
	gl_FragColor = vec4( finalFragment, finalColor.a );
}
