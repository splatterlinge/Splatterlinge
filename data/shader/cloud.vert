#version 120

varying vec3 vVertex;
varying vec3 vViewDir;
varying vec3 vSunDir;


void main()
{
	vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
	vViewDir = gl_Vertex.xyz;
	vSunDir = gl_LightSource[0].position.xyz;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
}
