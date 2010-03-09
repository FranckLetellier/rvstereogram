uniform sampler2D texture;
uniform int choix,windowwidth,windowheight,blurValue;
float sX, sY;
float PI = 3.14159265358979323846264;

uniform float lambda, sigma;

/******************************************************/
/*************** GAUSSIAN FILTER 1D ******************/
/****************************************************/

float gaussian(float x){

	return (lambda/(sigma*sqrt(2.0*PI)))*exp(-0.5*pow(x/sigma,2.0));

}

vec4 gaussianFilter1DVertical(int radius){

	vec4 finalColor;
	vec2 pos;
	float gaussianValue = 0.0;
	float gaussianTmp = 0.0;
	int index = 0;


	for(int j = -radius; j <= radius; j++){
		pos = vec2(gl_TexCoord[0].x,gl_TexCoord[0].y+float(j)*sY);
		gaussianTmp =  gaussian(j);
		gaussianValue += gaussianTmp;
		finalColor += texture2D(texture, pos) * gaussianTmp;
		index ++;
	}

	return finalColor/gaussianValue;

}

vec4 gaussianFilter1DHorizontal(int radius){

	vec4 finalColor;
	vec2 pos;
	float gaussianValue = 0.0;
	float gaussianTmp = 0.0;
	int index = 0;

	for(int i = -radius; i <= radius; i++){
		pos = vec2(gl_TexCoord[0].x+float(i)*sX,gl_TexCoord[0].y);
		gaussianTmp =  gaussian(i);
		gaussianValue += gaussianTmp;
		finalColor += texture2D(texture, pos) * gaussianTmp;
		index ++;
	}

	return finalColor/gaussianValue;

}



void main(){

	sX = 1.0/float(windowwidth);
	sY = 1.0/float(windowheight);


	if (choix == 0)
		gl_FragColor = gaussianFilter1DVertical(blurValue);
	else if (choix == 1)
		gl_FragColor = gaussianFilter1DHorizontal(blurValue);
		
	/* The FOLLOWING code is correct, but won't work on old graphics card
	-> need GLSL 1.30
	switch(choix){
		case 0 : 
			gl_FragColor = gaussianFilter1DVertical(10);
		break;
		case 1 : 
			gl_FragColor = gaussianFilter1DHorizontal(10);
		break;
	}
	*/
	
}
