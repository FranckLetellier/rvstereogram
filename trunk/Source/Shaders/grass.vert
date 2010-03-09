vec4 position;
vec4 new_position;

varying vec3 lightV, eyeV,normalV;
varying vec4 ambient;

uniform float angle;
uniform int sens;

void main(){

	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	position = gl_Vertex;
	new_position = position;
	
	vec4 dir = vec4(0.05,0.0,0.01,1.0);
	
	if(position.y > 0.1){		
		new_position.x += sens*(cos(angle)*0.05);
		new_position.z += sens*(sin(angle)*0.05);
	}
	
	
	gl_Vertex = new_position;

	//normal
	
	normalV = gl_NormalMatrix * vec3(0.0,1.0,0.0);

	// vertex position in eye space
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	//ambiant color
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient ;

	//from vertex to light
	lightV = gl_LightSource[0].position.xyz;

	//from vertex to eye
	eyeV =  - vVertex;


	gl_Position = ftransform();

}
