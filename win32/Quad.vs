attribute vec3 aPos;
attribute vec2 aTexCoord;


varying vec2 vTexCoord;


void main()
{
	gl_Position.xyz = aPos.xyz;
	gl_Position.w = 1.0;

	vTexCoord = aTexCoord;
}