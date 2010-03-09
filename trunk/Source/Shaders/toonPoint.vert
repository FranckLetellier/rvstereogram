varying vec3 normal,eyeV;
varying vec3 lightDir;
void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	normal = gl_NormalMatrix * gl_Normal;
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	//Vecteur du sommet vers la caméra
	eyeV =  - vVertex;

	//Vecteur du sommet vers la lumiere;
	lightDir = gl_LightSource[0].position.xyz - vVertex;
	gl_Position = ftransform();

}
