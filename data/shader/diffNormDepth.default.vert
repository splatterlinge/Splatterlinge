#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vVertex;
varying vec3 vLightPos[MAX_LIGHTS];


void main()
{
	vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
	vVertex = vec3( vertex );
	gl_ClipVertex = vertex;
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	vNormal = gl_NormalMatrix * gl_Normal;
	gl_FrontColor = gl_Color;

	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		vLightPos[i] = gl_LightSource[i].position.xyz - gl_LightSource[i].position.w * vVertex;
	}
}
