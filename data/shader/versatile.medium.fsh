#version 120
#define MAX_LIGHTS 2

varying vec3 vNormal, vViewDir;
varying vec3 vLightDir[MAX_LIGHTS];
varying float vAtt[MAX_LIGHTS];

uniform int activelights;

uniform sampler2D diffusemap;
uniform sampler2D specularmap;
uniform sampler2D normalmap;


void main()
{
	vec4 colorFromMap = texture2D( diffusemap, gl_TexCoord[0].st );
	vec4 specularFromMap = texture2D( specularmap, gl_TexCoord[0].st );
	vec3 final_color = gl_FrontMaterial.emission.rgb + gl_FrontMaterial.ambient.rgb * colorFromMap.rgb;
	vec3 viewDir = normalize( vViewDir );
	vec3 normal = normalize( vNormal );

	// recalculate normal based on normalmap
	vec3 normalFromMap = texture2D( normalmap, gl_TexCoord[0].st ).rgb * 2.0 - 1.0;
	vec3 dpx = dFdx( viewDir );
	vec3 dpy = dFdy( viewDir );
	vec2 dtx = dFdx( gl_TexCoord[0].st );
	vec2 dty = dFdy( gl_TexCoord[0].st );
	vec3 tangent = normalize( dpx * dty.t - dpy * dtx.t );
	vec3 binormal = normalize( -dpx * dty.s + dpy * dtx.s );
	mat3 TBN = mat3( tangent, binormal, normal );	// the transpose of texture-to-eye space matrix
	normal = normalize( TBN * normalFromMap );	// transform the normal to eye space

	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		final_color += gl_LightSource[i].ambient.rgb * gl_FrontMaterial.ambient.rgb * colorFromMap.rgb;

		vec3 L = normalize( vLightDir[i] );
		float lambertTerm = dot( normal, L );

		if( lambertTerm > 0.0 )
		{
			final_color +=
				gl_LightSource[i].diffuse.rgb *
				gl_FrontMaterial.diffuse.rgb *
				lambertTerm * vAtt[i] * colorFromMap.rgb;

			vec3 E = viewDir;
			vec3 R = reflect( -L, normal );
			float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess * specularFromMap.a );

			final_color +=
				gl_LightSource[i].specular.rgb *
				gl_FrontMaterial.specular.rgb *
				specular * vAtt[i] * specularFromMap.rgb;
		}
	}

	gl_FragColor = vec4( final_color, colorFromMap.a );
}
