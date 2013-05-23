#version 120

varying vec3 vViewDir;

uniform vec3 sunDir;
uniform float timeOfDay;
uniform float sunSpotPower;

uniform sampler2D diffuseMap;


// http://codeflow.org/entries/2011/apr/13/advanced-webgl-part-2-sky-rendering/
float phase( float alpha, float g )
{
	float a = 3.0*(1.0-g*g);
	float b = 2.0*(2.0+g*g);
	float c = 1.0+alpha*alpha;
	float d = pow( 1.0+g*g-2.0*g*alpha, 1.5 );
	return (a/b)*(c/d);
}


void main()
{
	vec3 viewDir = normalize( vViewDir );
	float alpha = dot( viewDir, normalize(sunDir) );
	float spot = smoothstep( 0.0, 15.0, phase( alpha, 0.9995 ) ) * sunSpotPower;
	vec4 skyColor = texture2D( diffuseMap, vec2( timeOfDay, viewDir.y ) );
	gl_FragColor = skyColor + skyColor*spot;
}
