#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vViewDir;
varying vec3 vLightDir[MAX_LIGHTS];
varying float vAtt[MAX_LIGHTS];

void main()
{
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = ftransform();
	vNormal = gl_NormalMatrix * gl_Normal;
	vec3 v = vec3(gl_ModelViewMatrix * gl_Vertex);
	vViewDir = normalize(-v);
	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		
		if( gl_LightSource[i].position.w == 0 )
		{
			vLightDir[i] = normalize( gl_NormalMatrix * gl_LightSource[i].position.xyz );
			vAtt[i] = 1.0;
		}
		else
		{
			vLightDir[i] = gl_NormalMatrix * gl_LightSource[i].position.xyz + vViewDir;
			float d = length( vLightDir[i] );
			vAtt[i] = 1.0 / (
				gl_LightSource[i].constantAttenuation +
				gl_LightSource[i].linearAttenuation * d +
				gl_LightSource[i].quadraticAttenuation * d*d );
		}
	}
}
