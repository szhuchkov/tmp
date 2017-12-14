
attribute vec3 aPos;
attribute vec2 aTexCoord;
attribute vec4 aColor;

uniform mat4 gWorld;
uniform mat4 gViewProjection;

varying vec4 vColor;
varying vec2 vTexCoord;

void main()
{
	// world position
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;

	// output position
	gl_Position = gViewProjection * vec4(wPos, 1.0);

	// output texcoord
	vTexCoord = aTexCoord;

	// output color
	vColor = aColor;
}
