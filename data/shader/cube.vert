#version 120


void main()
{
	gl_TexCoord[0] = gl_Vertex;
	gl_Position = ftransform();
	gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
}
