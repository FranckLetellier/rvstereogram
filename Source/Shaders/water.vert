varying vec3 lightV;
varying vec3 eyeV;

varying vec2 waterTex1;
varying vec2 waterTex2;
varying vec4 projectionCoordinates;

uniform float timer; // Time in millisecond

mat3 GetLinearPart( mat4 m )
{
	mat3 result;
	
	result[0][0] = m[0][0]; 
	result[0][1] = m[0][1]; 
	result[0][2] = m[0][2]; 

	result[1][0] = m[1][0]; 
	result[1][1] = m[1][1]; 
	result[1][2] = m[1][2]; 
	
	result[2][0] = m[2][0]; 
	result[2][1] = m[2][1]; 
	result[2][2] = m[2][2]; 
	
	return result;
}	


void main()
{
	//Il faut se mettre dans la TBN
	//T : tangente
	//B : Binormal
	//N : Normal
	//B = T (produitvectoriel) N
	
	//On le fait en dur, mais ca passe
	vec3 T = vec3(0.0, 1.0, 0.0);
	vec3 N = vec3(0.0, 0.0, 1.0);
	vec3 B = vec3(1.0, 0.0, 0.0);
	

	mat3 normalMatrix = gl_NormalMatrix;
	
	//On met ces vecteur dans le repere camera
	T = normalize(normalMatrix * T);
	B = normalize(normalMatrix * B);
	N = normalize(normalMatrix * N);

	mat3 TBNmat;
	TBNmat[0][0] = T[0];TBNmat[1][0] = T[1];TBNmat[2][0] = T[2];
	TBNmat[0][1] = B[0];TBNmat[1][1] = B[1];TBNmat[2][1] = B[2];
	TBNmat[0][2] = N[0];TBNmat[1][2] = N[1];TBNmat[2][2] = N[2];
	
	
	//vertex
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	//Vecteur du sommet vers la lumiere;
	
	//vec3 l = vec3(20.0,10.0,0.0);
	//lightV = l - gl_Vertex;
	
	lightV =  gl_LightSource[0].position.xyz- vVertex;

	//Vecteur du sommet vers la caméra
	eyeV =  - vVertex;
	//eyeV =  - gl_Vertex;

	//Repere TBN
	eyeV = TBNmat * eyeV;
	lightV = TBNmat * lightV;

	//decalage des coordonée pour faire avancé l'eau (eau + spéculaire)
	vec2 t1 = vec2(-timer*0.00002, -timer*0.00002);
    vec2 t2 = vec2(timer*0.00005, -timer*0.00005);
    
    waterTex1 = gl_MultiTexCoord0.xy*11 + t1;
    waterTex2 = gl_MultiTexCoord0.xy*16 + t2;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	projectionCoordinates = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	gl_Position = ftransform();
}
