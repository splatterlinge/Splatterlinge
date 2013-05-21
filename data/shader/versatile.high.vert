#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vVertex;
varying vec3 vLightDir[MAX_LIGHTS];
varying float vAtt[MAX_LIGHTS];

void main()
{
	vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
	vVertex = vec3( vertex );
	gl_ClipVertex = vertex;
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	vNormal = gl_NormalMatrix * gl_Normal;

	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		
		if( gl_LightSource[i].position.w == 0 )
		{
			vLightDir[i] = normalize( gl_LightSource[i].position.xyz );
			vAtt[i] = 1.0;
		}
		else
		{
			vec3 relPos = gl_LightSource[i].position.xyz - vVertex;
			vLightDir[i] = normalize( relPos );
			float d = length( relPos );
			vAtt[i] = 1.0 / (
				gl_LightSource[i].constantAttenuation +
				gl_LightSource[i].linearAttenuation * d +
				gl_LightSource[i].quadraticAttenuation * d*d );
		}
	}
}
