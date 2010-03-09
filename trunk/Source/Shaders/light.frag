varying vec3 normalV, lightV, eyeV;
varying vec3 lightV_TBN, eyeV_TBN, lightD_TBN;
varying vec4 s;
varying vec4 ambient;

uniform sampler2D texture; // regular texture
uniform sampler2D shadowMap; // depth map
uniform sampler2D normalMap; //for bumpmapping with Height map for parallax et POM on alpha channel

uniform float cosOut;
uniform bool useTexture, useShadowMap;
uniform bool usePOM;
uniform bool useBump;
uniform bool useParallax;
uniform float fSize ; //Size of the tile
uniform float heightFactor; // for parallax
uniform bool isDiscard;


float spot = 1.0;

/********************************************/
/***** Parallax Occlusion Mapping ***********/
/*******************************************/

vec2 POM(vec3 E_TBN, vec2 uv)
{
	
	//calculate the offset (like regular parallax)
	vec3 E = normalize(E_TBN);
	vec2 vOffset;
	vOffset.x = heightFactor * -E.x/E.z;
	vOffset.y = heightFactor * E.y/E.z;
	
	float num_steps_lin = 10.0; //Number of iteration to find a match for the depth
	float num_steps_bin = 15.0;	//number of step to find the corect height between the best two
	
	float linear_step = 1.0 / num_steps_lin;
	float depth = 0.0; // current depth position
	
	// best match found (starts with last position 1.0)
	float best_depth = 1.0;
	float step = linear_step;
	
	// search from front to back for first point inside the object
	for(float i=0; i<num_steps_lin-1.0; ++i)
	{
		depth += step;
		float h = 1.0 - texture2D(normalMap, uv+vOffset*depth).a;
		
		if (depth >= h) { 
			best_depth = depth; // store best depth
			i = num_steps_lin-1.0; // to go out of the "for" insctruction
		}
		
	}
	
	
	step = linear_step/2.0;
	depth = best_depth - step;
	
	//Dichotomie
	for(int i=0; i<num_steps_bin; i++)
	{
		float h = 1.0 - texture2D(normalMap, uv+vOffset*depth).a;
		
		step /= 2.0;
		best_depth = depth;
		
		if (depth >= h) 
			depth -= step;
		else 
			depth += step;
		
	}
	//Return the offset value for the textcoord
	return best_depth * vOffset;
}

/*******************************************/
/*****		Parrallax Mapping		 ***********/
/*******************************************/
vec2 parallaxMapping(vec2 uv, vec3 E_TBN)
{
	vec3 E = normalize(E_TBN);
	//distance offset
	float o = (texture2D(normalMap, uv).a - 0.5 ) * heightFactor;
	vec2 offset = o * (E.xy/E.z);
	
	return offset;
}

/*******************************************/
/*****		Normal Mapping		 ***********/
/*******************************************/
vec3 bumpMapping(vec2 uv, vec2 offsetCoord)
{
	vec2 newUV = uv+offsetCoord;
	vec3 newNormals = normalize((texture2D(normalMap,newUV).xyz *2.0 - 1.0 )) ;
	return newNormals;
}

/*******************************************/
/*****		Main				 ***********/
/*******************************************/
void main(){

	vec2 initialTexCoord = gl_TexCoord[0].xy*fSize;
	
	float shadow = 1.0;
	
	if(isDiscard)
	{
		if(texture2D(texture,initialTexCoord).a < 0.1)
		discard;
	}
	
	if(useShadowMap){
		vec3 shadowCoordinateWdivide;
		shadowCoordinateWdivide = s.xyz / s.w ;
		shadowCoordinateWdivide = shadowCoordinateWdivide * 0.5 + 0.5;

		float distanceFromLight = texture2D(shadowMap,shadowCoordinateWdivide.st).z;
		distanceFromLight += 0.00005;
 		
 		if (/*shadowCoordinateWdivide.s > 0.1 && shadowCoordinateWdivide.s < 0.99 && shadowCoordinateWdivide.t > 0.1 && shadowCoordinateWdivide.t < 0.99 && */s.w > 0.0)
 			shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;
 	}


			
	//common variable definition
	vec2 finalTexCoord;
	vec3 N,L,E;
		
		
	if (usePOM)
	{
		vec2 vfinalOffset = POM(eyeV_TBN, initialTexCoord);
		N = bumpMapping(initialTexCoord,vfinalOffset); // new normals
		finalTexCoord = initialTexCoord+ vfinalOffset; // nex textcoord
		L = normalize(lightV_TBN);	//We need the light in the TBN space
		E = normalize(eyeV_TBN);
	}
	else if(useParallax)
	{
		vec2 vfinalOffset = parallaxMapping(initialTexCoord,eyeV_TBN);
		N = bumpMapping(initialTexCoord,vfinalOffset); // new normals
		finalTexCoord = initialTexCoord+ vfinalOffset; // nex textcoord
		L = normalize(lightV_TBN);	//We need the light in the TBN space
		E = normalize(eyeV_TBN);
	}
	else if(useBump)
	{
		//Bump (normal) mapping
	
		N = bumpMapping(initialTexCoord,vec2(0.0)); // new normals
		L = normalize(lightV_TBN);	//We need the light in the TBN space
		E = normalize(eyeV_TBN);
		finalTexCoord = initialTexCoord.xy;
	}
	else
	{
		//Basic Phong Lighting
		
		N = normalize(normalV);
		L = normalize(lightV);
		finalTexCoord = initialTexCoord.xy;
		E = normalize(eyeV);
	}
	
	//if fragment is in shadow, we don't compute lighting
	if(shadow != 1.0){
	vec4 ambientColor = ambient;
		
		if(useTexture)
			ambientColor *= texture2D(texture,finalTexCoord);
			
		gl_FragColor =  ambientColor;
	}
	else{
		
		vec4 finalColor = ambient;
		
		
		/*********************************/
		/******** Spotlight *************/
		/*******************************/

		if(gl_LightSource[0].spotCutoff != 180.0)
		{
			vec3 D;
			if(usePOM || useBump){
				D = normalize(lightD_TBN);
			}
			else
				D = normalize(gl_LightSource[0].spotDirection);
			float cos_cur_angle = dot(-L, D);
			float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;

			float cos_inner_minus_outer_angle = cos_inner_cone_angle - cosOut;
			spot = clamp((cos_cur_angle - cosOut) / cos_inner_minus_outer_angle, 0.0, 1.0);
		}
			
		/*******************************/

		float coeffD = max(dot(N,L),0.0);
		if(coeffD > 0.0)
		{		
			//attenuation
			float d = length(lightV);
			float att = 1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation*d + gl_LightSource[0].quadraticAttenuation*d*d);
			
			
			vec3 R = reflect(-L, N);

			vec4 diffuse = coeffD * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse ;	

			vec4 specular = pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess) * gl_FrontMaterial.specular * gl_LightSource[0].specular ;

			finalColor += (diffuse + specular) * spot * att;
			
		}

		if(useTexture)
		{
			finalColor *= texture2D(texture,finalTexCoord);
		}
				
			gl_FragColor =   finalColor;
	}
		
}
