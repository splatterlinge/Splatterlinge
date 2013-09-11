#version 120

varying vec3 vVertex;
varying vec3 vNormal;
varying vec4 vTexCoord;

uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform sampler2D waterMap;
uniform float time;

void main()
{
	vec3 normal = normalize(vNormal);
	vec3 viewDir = normalize(-vVertex);
	float fangle = 1+abs(dot( viewDir, normal ));
	fangle = pow( fangle, 2 );
	float fresnelTerm = 1/fangle;

	vec2 texCoord = vec2( vTexCoord.x / vTexCoord.w, vTexCoord.y / vTexCoord.w );
	vec2 samplePos = vec2(256, 255) / 4 + time * 8 * vec2(0,1);
	vec3 bump = vec3( texture2D( waterMap, gl_TexCoord[0].st/32 + samplePos ) );
	vec2 perturbation = texCoord + 2 * (bump.rg - 0.5f);
	vec2 refrPerturbation = texCoord + 0.5 * (bump.rg - 0.5f);

	vec3 reflection = vec3( texture2D( reflectionMap, perturbation ) );
	vec3 refraction = vec3( texture2D( refractionMap, refrPerturbation ) );

	vec3 finalColor = mix( reflection, refraction, (1-fresnelTerm) );
	float fogFactor = clamp( -(length( vVertex )-gl_Fog.start) * gl_Fog.scale, 0.0, 1.0 );
	vec3 finalFragment = mix( gl_Fog.color.rgb, finalColor, fogFactor );

	gl_FragColor = vec4( finalFragment, 1 );
}
