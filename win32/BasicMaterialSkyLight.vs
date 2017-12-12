attribute vec3 aPos;
attribute vec2 aTexCoord;
attribute vec3 aNormal;

uniform mat4 gWorld;
uniform mat4 gViewProjection;
uniform mat4 gLightMatrix;

uniform vec4 gUniform0;
uniform vec4 gUniform1;

varying vec2 vTexCoord;
varying vec3 vColor;

void main()
{
	// world position
	vec3 wPos = (gWorld * vec4(aPos, 1.0)).xyz;

	// output position
	gl_Position = gViewProjection * vec4(wPos, 1.0);

    // world space normal
    vec3 wNormal = mat3(gWorld) * vec3(aNormal);

    // world space light direction
    vec3 lightDir = (gLightMatrix[0]).xyz;

    // diffuse part
    float diffPart = dot(lightDir, wNormal);
    diffPart = clamp(diffPart, 0.0, 1.0);

	// ambient part
	float ambientPart = dot(vec3(0, 1, 0), wNormal);
	ambientPart = 0.5 + 0.5 * ambientPart;

	// output texcoord
	vTexCoord = aTexCoord;

	// output color
	vColor = gUniform0.xyz * diffPart + gUniform1.xyz * ambientPart;
}
