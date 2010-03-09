varying vec3 normalV, lightV, eyeV;
varying vec3 lightV_TBN, eyeV_TBN, lightD_TBN;
varying vec4 s ;
varying vec4 ambient;

uniform mat4 eyematrix;
uniform mat4 transmatrix;

uniform bool tree;
uniform float timer;

void main(){

	// vertex position in eye space
	vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
	if (tree)
	{
		//vVertex.x +=  0.01 * pow(vVertex.y, 2.0) * (timer*0.0001);
	}

	//Getting back the Tangent and binormal
	vec3 T = normalize(gl_MultiTexCoord1.xyz); 
	
	//We are putting those into the world space, int order to create a matrix
	//that will convert light position and vertex position into the TBN space
	T = normalize(gl_NormalMatrix * T);
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec3 B = normalize(cross(N,T));

	mat3 TBNmat;
	TBNmat[0][0] = T[0];TBNmat[1][0] = T[1];TBNmat[2][0] = T[2];
	TBNmat[0][1] = B[0];TBNmat[1][1] = B[1];TBNmat[2][1] = B[2];
	TBNmat[0][2] = N[0];TBNmat[1][2] = N[1];TBNmat[2][2] = N[2];


	// vertex position in light space (for shadow mapping)
	s = eyematrix * transmatrix * gl_Vertex;
	
	// texture coordinates
	gl_TexCoord[0] =  gl_MultiTexCoord0;

	//normal
	normalV = gl_NormalMatrix * gl_Normal;
	


	//ambiant color
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient ;

	//from vertex to light
	lightV = gl_LightSource[0].position.xyz;
	//point or spot light
	if(gl_LightSource[0].position.w == 1.0)
		lightV -= vVertex.xyz;

	//from vertex to eye
	eyeV =  -vVertex.xyz;

	//TBN value
	eyeV_TBN = TBNmat * eyeV;
	lightV_TBN = TBNmat * lightV;
	lightD_TBN = TBNmat * gl_LightSource[0].spotDirection.xyz;
	
	gl_Position = ftransform();
	//gl_Position = gl_ModelViewProjectionMatrix * vVertex;
}
