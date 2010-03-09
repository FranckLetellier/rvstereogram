uniform sampler2D depthMap;
uniform sampler2D texture;
float depth;

vec4 dofParams;

float computeDepthBlur(float depth){

	float f;

	if(depth < dofParams.y){
		f = (depth - dofParams.y) / (dofParams.y - dofParams.x);
		//f = clamp(f,-1.0,0.0);
	}
	else{
		f = (depth - dofParams.y) / (dofParams.z - dofParams.y);
		f = clamp(f,0.0,dofParams.w);
	}
	
	return f*0.5 + 0.5;

}

void main(){

	dofParams.x = 0.60;
	dofParams.y = 0.9;
	dofParams.z = 0.99;
	dofParams.w = 0.9;

	depth = texture2D(depthMap,gl_TexCoord[0].st).x;

	vec4 finalColor = texture2D(texture, gl_TexCoord[0].xy);
	finalColor.a = computeDepthBlur(depth);
	gl_FragColor = finalColor;

}
