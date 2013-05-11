#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vVertex;
varying vec3 vLightDir[MAX_LIGHTS];
varying float vAtt[MAX_LIGHTS];

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D blobMap;


void main()
{
	vec4 colorFromMap = texture2D( diffuseMap, gl_TexCoord[0].st );
	vec4 specularFromMap = texture2D( specularMap, gl_TexCoord[0].st );
	vec3 finalColor = gl_FrontMaterial.emission.rgb;
	vec3 viewDir = normalize( -vVertex );
	vec3 normal = normalize( vNormal );

	// recalculate normal based on normalmap
	vec3 normalFromMap = normalize( texture2D( normalMap, gl_TexCoord[0].st ).rgb * 2.0 - 1.0 );
	vec3 dpx = dFdx( vVertex );
	vec3 dpy = dFdy( vVertex );
	vec2 dtx = dFdx( gl_TexCoord[0].st );
	vec2 dty = dFdy( gl_TexCoord[0].st );
	vec3 tangent = -normalize( dpx * dty.t - dpy * dtx.t );
	vec3 binormal = normalize( -dpx * dty.s + dpy * dtx.s );
	mat3 TBN = mat3( tangent, binormal, normal );	// the transpose of texture-to-eye space matrix
	normal = normalize( TBN * normalFromMap );	// transform the normal to eye space

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
			float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess * specularFromMap.a + 1 );

			finalColor +=
				gl_LightSource[i].specular.rgb *
				gl_FrontMaterial.specular.rgb *
				specular * vAtt[i] * specularFromMap.rgb;
		}
	}

	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );
	gl_FragColor = vec4( finalFragment, colorFromMap.a * texture2D( blobMap, gl_TexCoord[1].st ).r );
}
