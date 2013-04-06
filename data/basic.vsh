#version 120
#define MAX_LIGHTS 1

varying vec3 normal, eyeVec;
varying vec3 lightDir[MAX_LIGHTS];
varying float att[MAX_LIGHTS];

void main()
{
	gl_Position = ftransform();
	normal = gl_NormalMatrix * gl_Normal;
	vec4 v = gl_ModelViewMatrix * gl_Vertex;
	eyeVec = -v.xyz;
	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		lightDir[i] = vec3( gl_LightSource[i].position.xyz + eyeVec );
		float d = length( lightDir[i] );
		att[i] = 1.0 / ( gl_LightSource[i].constantAttenuation +
			(gl_LightSource[i].linearAttenuation * d) +
			(gl_LightSource[i].quadraticAttenuation * d*d) );
	}
}
