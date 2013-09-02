#version 120

varying vec3 vVertex;
varying vec3 vEyeVector;
varying vec3 vNormal;

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform sampler2D waterMap;
uniform float time;

void main()
{
	float fangle = 1+dot( normalize(vEyeVector), normalize(vNormal) );
	fangle = pow(fangle ,5);
	float fresnelTerm = 1/fangle;

	//just to be sure that the value is between 0 and 1;
	fresnelTerm = max( 0, fresnelTerm );
	fresnelTerm = min( 1, fresnelTerm );

	vec3 bump = vec3( texture2DProj( waterMap, gl_TexCoord[0] ) );
	//                     frequency range
	//                           |    |
	bump.x = bump.x + sin(time/1000)/16;
	bump.y = bump.y + cos(time/1000)/16;
	vec2 perturbation = 5 * (bump.rg - 0.5f);
	vec2 texCoord = vec2( gl_TexCoord[0].x / gl_TexCoord[0].w, gl_TexCoord[0].y / gl_TexCoord[0].w );
	vec2 perturbatedTexCoords = texCoord + perturbation;
	vec2 perturbatedRefrTexCoords = texCoord;
	vec3 reflection = vec3( texture2D( reflectionMap, perturbatedTexCoords ) );
	vec3 refraction = vec3( texture2D( refractionMap, perturbatedRefrTexCoords ) );

	vec3 finalColor = mix( reflection, refraction, 0.8 );
	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );

	gl_FragColor = vec4( finalFragment, 1 );
}
