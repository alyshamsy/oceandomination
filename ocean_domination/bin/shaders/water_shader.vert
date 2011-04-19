//this is the vertex shader to generate waves 

uniform float time;
uniform float wind_factor;
uniform int movement;
uniform vec3 ship_location;

void main()
{	
	float x_frequency = (float) ((gl_Vertex.x/5.0f)-4.5f);
	float z_frequency = (float) ((gl_Vertex.z/5.0f)-4.5f);
	
	float x_low = ship_location.x - 3.0f;
	float x_high = ship_location.x + 3.0f;
	float z_low = ship_location.z;
	float z_high = ship_location.z + 13.0f;

	vec4 temp = gl_Vertex;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;

	if(gl_Vertex.x >= x_low && gl_Vertex.x <= x_high && gl_Vertex.z >= z_low && gl_Vertex.z <= z_high && movement != 0) {
		temp.y = wind_factor * 4 * sin(-(time + x_frequency*30.0)) * sin(-(time + z_frequency*30.0));
	} else {
		temp.y = wind_factor * sin(time + x_frequency*30.0) * sin(time + z_frequency*30.0);
	}

	gl_Position = gl_ModelViewProjectionMatrix * temp;
	gl_FrontColor = gl_Color;
}