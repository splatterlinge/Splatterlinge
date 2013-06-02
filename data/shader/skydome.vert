#version 120

varying vec3 vViewDir;


void main()
{
	vViewDir = gl_Vertex.xyz;
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
}
