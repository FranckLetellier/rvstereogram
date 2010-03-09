uniform float mixValue;
varying vec3 pos;
uniform samplerCube day;
uniform samplerCube night;
void main(void)
{
	gl_FragColor = mix( textureCube(day,normalize(pos).xyz),textureCube(night,normalize(pos).xyz),mixValue);
}
