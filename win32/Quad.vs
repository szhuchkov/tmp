attribute vec3 aPos;


uniform vec4 gUniform0;


varying vec2 vTexCoord;


void main()
{
	gl_Position.xy = 2.0 * (gUniform0.xy + aPos.xy * gUniform0.zw) - 1.0;
	gl_Position.z = aPos.z;
	gl_Position.w = 1.0;

	vTexCoord.xy = aPos.xy;
}