varying vec3 lightV, eyeV,normalV;
varying vec4 ambient;

uniform sampler2D text_herbe;

void main(void)
{


	vec4 finalColor = ambient;
	vec4 color = texture2D(text_herbe, gl_TexCoord[0].st);
	if(color.a < 0.8) discard;

	vec3 N,L,E;

	N = normalize(normalV);
	L = normalize(lightV);
	E = normalize(eyeV);

	float coeffD = max(dot(N,L),0.0);
	if(coeffD > 0.0)
	{		
	//attenuation
	float d = length(lightV);
	float att = 1.0/(gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation*d + gl_LightSource[0].quadraticAttenuation*d*d);
			
			
	vec3 R = reflect(-L, N);

	vec4 diffuse = coeffD * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse ;	

	finalColor += diffuse * att;
			
	}


		
	gl_FragColor = color * finalColor;

}