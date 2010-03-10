uniform sampler2D texture, texture2,depthTexture;
uniform int choix;
uniform float fWindowHeight;
uniform float fWindowWidth;
uniform float thresholdValue;


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
	else if(choix == 4){
		gl_FragColor = add();
	}
}
