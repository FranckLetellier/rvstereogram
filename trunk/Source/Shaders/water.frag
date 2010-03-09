uniform sampler2D reflexion;
uniform sampler2D refraction;
uniform sampler2D perlin;

varying vec3 lightV;
varying vec3 eyeV;

varying vec2 waterTex1;	//For the flow
varying vec2 waterTex2;	//For the ripple
varying vec4 projectionCoordinates; // For the reflection


float fresnelAlpha(vec4 normal, vec3 eye)
{
	float coef = 0.5;
	vec3 n = vec3(normal);
	return (1.0-coef) + pow((1.0 - dot(n*0.6,eye)), 1.5) * coef;
}

void main()
{
	//Value that will become uniform
	float scale = 0.1; //deformation de l'environement
	float scale2 = 0.02; //forces des remous
	float scale3 = 0.1; // Ralentir les vague
	
	vec3 E = normalize(eyeV);
	
	//---Calcul des normal
	vec2 rippleEffect = scale2 * texture2D(refraction, waterTex2 * scale3).xy;
	vec4 N = texture2D(perlin, waterTex1 + rippleEffect);
	
	//On passe les normals dans un optique de [-1;1] plutot que [0;1]
	N = (N-vec4(0.5,0.5,0.5,1.0)) * vec4(2.0,2.0,2.0,1.0);
	N = normalize(N); 

	//---Reflection 
	vec2 fdist = texture2D(refraction, waterTex1 + rippleEffect).xy;
	fdist = fdist * 2.0 - vec2(1.0);
	fdist *= scale;
	
	//calculate specular highlight
	vec3 L = normalize(lightV);
	vec3 vRef = normalize(reflect(-L, vec3(N)*0.6));
	float stemp = clamp(dot(E, vRef), 0.0, 1.0);
	stemp = pow(stemp, 40.0); 
	vec4 specular = vec4(stemp);

	//get projective texcoords
	vec2 projCoord = projectionCoordinates.xy / projectionCoordinates.w;
	projCoord = (projCoord + 1.0) * 0.5;
	projCoord += fdist.xy; // Permet de faire suivre le mouvement des normal à la reflexion
	projCoord = clamp(projCoord, 0.001, 0.999); 
	
	//load and calculate reflection
	vec4 refl = texture2D(reflexion, projCoord);

	gl_FragColor =  refl + specular;
	gl_FragColor.a =  fresnelAlpha(N,E);

	
	//AutreMethodes
	//gl_FragColor = texture2D(reflexion,vec2(gl_FragCoord.x/1024.0, gl_FragCoord.y/768.0) + (N*0.1));




	//Alpha fog 
	/*
	float fogFactor = exp2(-(0.03*projectionCoordinates.z));
	fogFactor = mod(fogFactor,1.0);
	gl_FragColor.a = fogFactor*(gl_FragColor.a);
	*/
	

}
