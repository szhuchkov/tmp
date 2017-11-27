
attribute vec3 aPos;
attribute vec2 aTexCoord;
attribute vec4 aColor;

uniform mat4 gWorld;
uniform mat4 gViewProjection;

varying vec2 vTexCoord;

void main()
{
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;
	gl_Position = gViewProjection * vec4(wPos, 1.0);

	vTexCoord = aTexCoord;
}
