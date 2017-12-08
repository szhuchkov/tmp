
attribute vec3 aPos;
attribute vec2 aTexCoord;

uniform mat4 gWorld;
uniform mat4 gViewProjection;

varying vec2 vTexCoord;
varying vec4 vColor;

void main()
{
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;
	gl_Position = gViewProjection * vec4(wPos, 1.0);

	vTexCoord = aTexCoord;
	vColor = vec4(1.0, 1.0, 1.0, 1.0);
}
