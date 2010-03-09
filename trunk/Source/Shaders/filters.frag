uniform sampler2D texture, texture2,depthTexture;
uniform int choix;
uniform mat4 inverseModelProjection;
uniform mat4 previousModelProjection;
uniform float fWindowHeight;
uniform float fWindowWidth;
uniform float thresholdValue;

vec4 motionBlur()
{
	vec4 tmp1,tmp2; 
	vec2 UV = gl_TexCoord[0].xy;
	//Retrieve depth of pixel  
	float z = texture2D(depthTexture, UV).z;  
	
	//Simplified equation of GluUnproject
	vec4 currentPos = vec4( 2.0* (gl_FragCoord.x/fWindowHeight)  - 1.0, 2.0* (gl_FragCoord.y/fWindowWidth) - 1.0, 2.0*z -1.0 , 1.0);

	//Back into the worldSpace 
	tmp1 =  currentPos  * inverseModelProjection  ;  
	
	//Homogenous value 
	vec4 posInWorldSpace = tmp1/tmp1.w;  
	
	//Using the world coordinate, we transform those into the previous frame
	tmp2 =  previousModelProjection *posInWorldSpace ;  
	vec4 previousPos = tmp2/tmp2.w;  
	
	//Compute the frame velocity using the difference 
	vec2 velocity = ((currentPos - previousPos)/10.0).xy;

	//Get the initial color at this pixel.  
	vec4 originalColor = texture2D(texture, UV);
	UV += velocity.xy;  
	for(int i = 1; i < 20.0; ++i)  
	{  
		//Sample the color buffer along the velocity vector.  
		vec4 currentColor = texture2D(texture, UV);  
		//Add the current color to our color sum.  
		originalColor += currentColor;  
		UV.x += velocity.x;
		UV.y += velocity.y;
	}  
	//Average all of the samples to get the final blur color.  
	return originalColor / 20.0;  
	
}

vec4 inverse()
{
	vec4 originalColor =  texture2D(texture, gl_TexCoord[0].xy);
	vec4 finalColor;
	finalColor.rgb =vec3( 1.0 - originalColor.rgb );
	finalColor.a = originalColor.a;
	return finalColor;
}

vec4 toGray()
{
	vec4 originalColor =  texture2D(texture, gl_TexCoord[0].xy);
	vec4 finalColor;
	finalColor.rgb = vec3(originalColor.r * 0.299 + originalColor.g * 0.587 + originalColor.b * 0.114);
	finalColor.a = originalColor.a;
	return finalColor;
}

vec4 threshold()
{
	vec4 originalColor =  texture2D(texture, gl_TexCoord[0].xy);
	if((originalColor.r + originalColor.g + originalColor.b)/3.0 > thresholdValue)
		return vec4(1.0);
	
	return vec4(0.0,0.0,0.0,1.0);
}

vec4 add()
{
	return texture2D(texture, gl_TexCoord[0].xy) + texture2D(texture2, gl_TexCoord[0].xy);
}

void main()
{

	if(choix == 0){
		gl_FragColor = texture2D(texture , gl_TexCoord[0].xy);
		//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	}
	else if(choix == 1){
		gl_FragColor = toGray();
	}
	else if(choix == 2){
		gl_FragColor = inverse();
	}
	else if(choix == 3){
		gl_FragColor = threshold();
	}
	else if(choix == 4){
		gl_FragColor = add();
	}
	else if(choix == 5){
		gl_FragColor = motionBlur();
	}
}
