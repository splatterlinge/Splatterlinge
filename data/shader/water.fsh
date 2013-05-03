#version 120


uniform sampler2D refractionMap;


void main()
{
	vec2 projectCoord = (vec2(gl_TexCoord[0]/gl_TexCoord[0].q)+ 1.0) * 0.5;
	gl_FragColor = vec4( texture2D( refractionMap, projectCoord ) );
}
