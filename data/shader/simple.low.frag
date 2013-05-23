#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vVertex;
varying vec3 vLightDir[MAX_LIGHTS];
varying float vAtt[MAX_LIGHTS];

uniform sampler2D diffuseMap;


void main()
{
	vec4 colorFromMap = texture2D( diffuseMap, gl_TexCoord[0].st );
	vec3 finalColor = gl_FrontMaterial.emission.rgb;
	vec3 viewDir = normalize( -vVertex );
	vec3 normal = normalize( vNormal );

	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		finalColor += gl_LightSource[i].ambient.rgb * gl_FrontMaterial.ambient.rgb * colorFromMap.rgb;

		vec3 L = normalize( vLightDir[i] );
		float lambertTerm = dot( normal, L );

		if( lambertTerm > 0.0 )
		{
			finalColor +=
				gl_LightSource[i].diffuse.rgb *
				gl_FrontMaterial.diffuse.rgb *
				lambertTerm * vAtt[i] * colorFromMap.rgb;

			vec3 E = viewDir;
			vec3 R = reflect( -L, normal );
			float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess + 1 );

			finalColor +=
				gl_LightSource[i].specular.rgb *
				gl_FrontMaterial.specular.rgb *
				specular * vAtt[i];
		}
	}

	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );
	gl_FragColor = vec4( finalFragment, colorFromMap.a );
}
