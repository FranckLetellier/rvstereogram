varying vec3 normalV, lightV, eyeV;
varying vec4 ambient;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D spec;
uniform sampler2D spec_texture;

uniform sampler2D bump;
//Height max of the heightmap
uniform float height_max;

varying vec3 height;

void main(void)
{
	
	float hauteur = height.z / height_max;
	
	vec4 finalColor = ambient;
	vec2 UV = gl_TexCoord[0].xy*50.0;


	vec3 N = normalize(normalV);	
	vec3 L = normalize(lightV);
	//N = normalize (texture2D(bump,UV*2.0).xyz *2.0 - 1.0) ;
	
	float coeffD = max(dot(N,L),0.0);
	
	if(coeffD > 0.0){
		vec4 diffuse = coeffD * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse ;	
		
		vec3 E = normalize(eyeV);
		vec3 R = reflect(-L, N);		
		
		vec4 specular = pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess) * gl_FrontMaterial.specular * gl_LightSource[0].specular ;
		
		//attenuation
		float d = length(lightV);
		float att = 1/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation*d + gl_LightSource[0].quadraticAttenuation*d*d);
		
		finalColor += (diffuse+specular) * att;
	}
	
	
	
	vec4 lowColor = texture2D(tex1,UV);
	vec4 middleColor = texture2D(tex2,UV) ;
	vec4 highColor = texture2D(tex3,UV) ;
	
	vec4 finalMix = mix(mix(lowColor,middleColor,hauteur*0.6),mix(middleColor,highColor,hauteur*0.4),hauteur);

	///To place certain texture at certain places
	float spec_coeff = texture2D(spec,gl_TexCoord[0].xy).r;
	finalMix = mix( finalMix, texture2D(spec_texture,UV*3.0),spec_coeff);
	
	
	gl_FragColor = finalMix*finalColor;
	
	

}
