// raycast - plain vertex shader
varying vec3 pos;
varying vec4 color;
uniform mat4 model;
varying vec2 tc;
varying  vec3 anormal;
varying vec4 aposition;
void main(void)
{
	
	tc = gl_MultiTexCoord0.st;
	pos = gl_Vertex.xyz;
	aposition =  gl_Vertex;
	anormal = gl_Normal.xyz;
	color = gl_Color;
	gl_Position = ftransform();
}
	