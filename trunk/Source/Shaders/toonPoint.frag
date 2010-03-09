varying vec3 normal,eyeV;
varying vec3 lightDir;

uniform sampler2D texture;
uniform bool useOutline, useTexture;

void main(void)
{

	float intensity;
	vec4 color = vec4(0.0,0.0,0.0,1.0);
	vec3 N = normalize(normal);
	vec3 E = normalize(eyeV);
	vec3 L = normalize(lightDir);
		
	intensity = dot(L,N);
	

	if (dot(N,E) < 0.3 && useOutline) gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	else{
		
		if(intensity > 0.98) color += 1.2 * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		else if(intensity > 0.5) color += gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		else if(intensity > 0.25) color += 0.5 * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		else color += 0.25 * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		if(useTexture) color *= texture2D(texture, gl_TexCoord[0].xy);
		gl_FragColor = color;
	}
}
