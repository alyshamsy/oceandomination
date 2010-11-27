//this is the vertex shader to generate waves 

uniform float time;
uniform float wind_factor;

void main()
{	
	float x_frequency = (float) ((gl_Vertex.x/5.0f)-4.5f);
	float z_frequency = (float) ((gl_Vertex.z/5.0f)-4.5f);
	vec4 temp = gl_Vertex;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;

	temp.y = wind_factor * sin(time + x_frequency*30.0) * sin(time + z_frequency*30.0);
	
	gl_Position = gl_ModelViewProjectionMatrix * temp;
	gl_FrontColor = gl_Color;
}