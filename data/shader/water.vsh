#version 120


void main()
{
	gl_TexCoord[0] = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}
