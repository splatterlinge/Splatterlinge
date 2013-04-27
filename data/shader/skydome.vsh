#version 120

varying vec3 vViewDir;

void main()
{
	vViewDir = normalize( gl_Vertex.xyz );
	gl_Position = ftransform();
}
