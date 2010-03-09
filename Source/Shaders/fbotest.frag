uniform sampler2D image;
uniform float choix;


vec4 inverse()
{
	vec4 originalColor =  texture2D(image , gl_TexCoord[0].xy);
	vec4 finalColor;
	finalColor.rgb =vec3( 1.0 - originalColor.rgb );
	finalColor.a = originalColor.a;
	return finalColor;
}

vec4 toGray()
{
	vec4 originalColor =  texture2D(image , gl_TexCoord[0].xy);
	vec4 finalColor;
	finalColor.rgb = vec3(originalColor.r * 0.299 + originalColor.g * 0.587 + originalColor.b * 0.114);
	finalColor.a = originalColor.a;
	return originalColor;
}



void main()
{
	if (choix == 1.0)
	{
		gl_FragColor = inverse();
	}
	else if (choix == 2.0)
	{
		gl_FragColor = toGray();
	}

}
