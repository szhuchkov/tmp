
attribute vec3 aPos;

uniform mat4 gWorld;
uniform mat4 gViewProjection;


varying vec4 vShadowPos;


void main()
{
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;
	gl_Position = vShadowPos = gViewProjection * vec4(wPos, 1.0);
}
