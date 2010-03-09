varying vec3 pos;
void main(void)
{
	pos = vec3(gl_Vertex);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();

}
