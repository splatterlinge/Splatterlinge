#version 120

varying vec3 vVertex;
varying vec3 vNormal;
varying vec4 vTexCoord;

void main()
{
	mat4 proj2Tex = mat4(
		0.5, 0.0, 0.0, 0.5,
		0.0, 0.5, 0.0, 0.5,
		0.0, 0.0, 0.5, 0.5,
		0.0, 0.0, 0.0, 1.0
	);
	vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
	vNormal = gl_NormalMatrix * gl_Normal;
	vTexCoord = (gl_ModelViewProjectionMatrix * gl_Vertex) * proj2Tex;
	gl_TexCoord[0].xy   = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}
