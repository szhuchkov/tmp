
attribute vec3 aPos;
attribute vec2 aTexCoord;

uniform mat4 gWorld;
uniform mat4 gViewProjection;

varying vec2 vTexCoord;
varying vec3 vColor;

void main()
{
	// world position
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;

	// output position
	gl_Position = gViewProjection * vec4(wPos, 1.0);

	// output texcoord
	vTexCoord = aTexCoord;

	// output color
	vColor = vec3(1.0);
}
