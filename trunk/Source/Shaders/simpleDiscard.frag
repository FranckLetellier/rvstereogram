uniform sampler2D texture;
void main(void)
{

	if(texture2D(texture,gl_TexCoord[0].xy).a < 0.1)
		discard;
}
