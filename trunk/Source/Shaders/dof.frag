uniform sampler2D texture,textureSmall;
uniform int windowwidth,windowheight;

vec2 poisson[8];
vec2 maxCoC = vec2(5.0,10.0);
float radiusScale = 0.5;


vec2 pixelSizeHigh, pixelSizeLow;
float depth;

vec4 dof(vec2 coords){

	vec4 finalColor;

	float discRadius, discRadiusLow, centerDepth;

	finalColor = texture2D(texture, coords);
	centerDepth = finalColor.a;

	discRadius = abs(centerDepth * maxCoC.y - maxCoC.x);
	discRadiusLow = discRadius * radiusScale;

	finalColor = vec4(0.0,0.0,0.0,0.0);

	for(int i = 0; i < 8; i++){

		vec2 coordLow = coords + (pixelSizeLow * poisson[i] * discRadiusLow);
		vec2 coordHigh = coords + (pixelSizeHigh * poisson[i] * discRadius);

		vec4 tapLow = texture2D(textureSmall, coordLow);
		vec4 tapHigh = texture2D(texture, coordHigh);

		float tapBlur = abs(tapHigh.a * 2.0 - 1.0);
		vec4 tap = mix(tapHigh,tapLow,tapBlur);

		tap.a = (tap.a >= centerDepth) ? 1.0 : abs(tap.a * 2.0 - 1.0);

		finalColor.rgb += tap.rgb * tap.a;
		finalColor.a += tap.a;

	}

	return finalColor/finalColor.a;


}

void main(){

	pixelSizeHigh[0] = 1.0/float(windowwidth);
	pixelSizeHigh[1] = 1.0/float(windowheight);

	pixelSizeLow[0] = 1.0/float(windowwidth/2);
	pixelSizeLow[1] = 1.0/float(windowheight/2);

	//this won't work etiher on older graphics cards
	// but I don't know how to fix it
	// poisson-distributed positions
	poisson = vec2[8](
	vec2( 0.0, 0.0),
	vec2( 0.527837,-0.085868),
	vec2(-0.040088, 0.536087),
	vec2(-0.670445,-0.179949),
	vec2(-0.419418,-0.616039),
	vec2( 0.440453,-0.639399),
	vec2(-0.757088, 0.349334),
	vec2( 0.574619, 0.685879)
	);

	gl_FragColor = dof(gl_TexCoord[0].xy);

}
