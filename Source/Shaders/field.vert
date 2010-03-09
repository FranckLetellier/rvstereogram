varying vec3 normalV, lightV, eyeV;
varying vec4 ambient;
varying vec3 height;


void main(void)
{
	//On met ces vecteur dans le repere camera
	normalV = normalize(gl_NormalMatrix * gl_Normal);
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	/* Tangent still unavailable
	vec3 T = normalize(gl_MultiTexCoord1.xyz); 
	T = normalize(mat3(gl_NormalMatrix) * T);
	vec3 N = normalize(mat3(gl_NormalMatrix) * gl_Normal);
	vec3 B = normalize(cross(N,T));

	mat3 TBNmat;
	TBNmat[0][0] = T[0];TBNmat[1][0] = T[1];TBNmat[2][0] = T[2];
	TBNmat[0][1] = B[0];TBNmat[1][1] = B[1];TBNmat[2][1] = B[2];
	TBNmat[0][2] = N[0];TBNmat[1][2] = N[1];TBNmat[2][2] = N[2];
	eyeV = TBNmat * eyeV;
	lightV= TBNmat * lightV;
	normalV= TBNmat * normalV;
	*/
	
	
	//Calcul de la lumière ambiante
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient ;
	
	//from vertex to light
	lightV = gl_LightSource[0].position.xyz;
	//point or spot light
	if(gl_LightSource[0].position.w == 1.0)
		lightV -= vVertex;
	
	//Vecteur du sommet vers la caméra
	eyeV =  - vVertex;
	

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	height = gl_Vertex.xyz;

}
