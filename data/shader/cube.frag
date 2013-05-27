#version 120

uniform samplerCube cubeMap;


void main()
{
	gl_FragColor = vec4( textureCube( cubeMap, gl_TexCoord[0].xyz ) );
}
