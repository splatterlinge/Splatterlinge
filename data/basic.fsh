#version 120
#define MAX_LIGHTS 1

varying vec3 normal, eyeVec;
varying vec3 lightDir[MAX_LIGHTS];
varying float att[MAX_LIGHTS];

void main (void)
{
	vec4 final_color;
	vec3 N = normalize( normal );
	for( int i=0; i<MAX_LIGHTS; ++i )
	{
		final_color =
			(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
			(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

		vec3 L = normalize( lightDir[i] );
		float lambertTerm = dot( N, L );

		if( lambertTerm > 0.0 )
		{
			final_color +=
				gl_LightSource[i].diffuse *
				gl_FrontMaterial.diffuse *
				lambertTerm * att[i];

			vec3 E = normalize( eyeVec );
			vec3 R = reflect( -L, N );
			float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );

			final_color +=
				gl_LightSource[i].specular *
				gl_FrontMaterial.specular *
				specular * att[i];
		}
	}
	gl_FragColor = final_color;
}
