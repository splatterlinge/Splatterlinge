#version 120

varying vec3 vVertex;

uniform sampler2D directMap;


void main()
{
	vec3 viewDir = normalize( -vVertex );
	vec4 finalColor = texture2D( directMap, gl_TexCoord[0].st ) * gl_Color;
	gl_FragColor = finalColor;
}
