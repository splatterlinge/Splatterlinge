#version 120


varying vec3 vVertex;


void main()
{
	vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
	vVertex = vec3( vertex );
	gl_TexCoord[0] = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}
