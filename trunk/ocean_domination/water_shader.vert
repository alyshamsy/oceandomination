//this is the vertex shader to generate waves 

uniform float time;
uniform float wind_factor;
uniform float x_frequency;
uniform float z_frequency;

void main()
{
	float time_scale = 0.001;
	
	vec4 temp = gl_Vertex;
	temp.y = gl_Vertex.y + 50;
	//temp.y = gl_Vertex.y + wind_factor * sin(time_scale*time + x_frequency*gl_Vertex.x) * sin(time_scale*time + z_frequency*gl_Vertex.z);

	gl_Position = gl_ModelViewProjectionMatrix * temp;
	gl_FrontColor = gl_Color;
}