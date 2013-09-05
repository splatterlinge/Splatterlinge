#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vVertex;
varying vec3 vLightPos[MAX_LIGHTS];

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;


void main()
{
	vec3 finalColor = gl_FrontMaterial.emission.rgb;
	vec3 normal = normalize( vNormal );
	vec3 viewDir = normalize( -vVertex );

	// calculate tangent space matrix
	vec3 dpx = dFdx( vVertex );
	vec3 dpy = dFdy( vVertex );
	vec2 dtx = dFdx( gl_TexCoord[0].st );
	vec2 dty = dFdy( gl_TexCoord[0].st );
	vec3 tangent = normalize( dpx * dty.t - dpy * dtx.t );
	vec3 binormal = normalize( -dpx * dty.s + dpy * dtx.s );
	mat3 TBN = mat3( tangent, binormal, normal );	// the transpose of texture-to-eye space matrix

	vec4 colorFromMap = texture2D( diffuseMap, gl_TexCoord[0].st ) * gl_Color;
	vec3 normalFromMap = normalize( texture2D( normalMap, gl_TexCoord[0].st ).rgb * 2.0 - 1.0 );
	normal = normalize( TBN * normalFromMap );	// transform the normal to eye space

	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		finalColor += gl_LightSource[i].ambient.rgb * gl_FrontMaterial.ambient.rgb * colorFromMap.rgb;

		vec3 lightDir = normalize( vLightPos[i] );
		float lambert = max( 0.0, dot( normal, lightDir ) );

		float d = length( vLightPos[i] );
		float attenuation = 1.0 / (
			gl_LightSource[i].constantAttenuation +
			gl_LightSource[i].linearAttenuation * d +
			gl_LightSource[i].quadraticAttenuation * d*d );

		finalColor +=
			gl_LightSource[i].diffuse.rgb *
			gl_FrontMaterial.diffuse.rgb *
			lambert * attenuation * colorFromMap.rgb;

		vec3 R = reflect( -lightDir, normal );
		float specular = pow( max(dot(R, viewDir), 0.0), gl_FrontMaterial.shininess + 1 );

		finalColor +=
			gl_LightSource[i].specular.rgb *
			gl_FrontMaterial.specular.rgb *
			specular * attenuation;
	}

	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );
	gl_FragColor = vec4( finalFragment, colorFromMap.a * gl_FrontMaterial.diffuse.a );
}
