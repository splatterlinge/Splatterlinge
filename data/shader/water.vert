#version 120

uniform vec3 eyeVector;

varying vec3 vVertex;
varying vec3 vEyeVector;
varying vec3 vNormal;

void main()
{
	mat4 proj2Tex = mat4(
		0.5, 0.0, 0.0, 0.5,
		0.0, 0.5, 0.0, 0.5,
		0.0, 0.0, 0.5, 0.5,
		0.0, 0.0, 0.0, 1.0
	);
	vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
	vVertex = vec3( vertex );
	vNormal = gl_Normal;
	gl_TexCoord[0] = (gl_ModelViewProjectionMatrix * gl_Vertex) * proj2Tex;
	gl_Position = ftransform();
}
