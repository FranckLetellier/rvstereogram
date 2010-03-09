uniform sampler2D texture, texture2;
uniform float pas, coeff,displacementValue;

void main(){

	gl_TexCoord[0] = gl_MultiTexCoord0;

	//vertex displacement
	vec3 v = vec3(gl_Vertex.x,((1.0-coeff)*texture2D(texture,gl_MultiTexCoord0.xy)+coeff*texture2D(texture2,gl_MultiTexCoord0.xy)).x*displacementValue, gl_Vertex.z);

	gl_Position = gl_ModelViewProjectionMatrix * vec4(v,1);

}
