uniform sampler2D tex;
attribute float texture_quadrant;
attribute float life;
varying float quadrant;
varying float lifetime;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0]  = gl_MultiTexCoord0;

	quadrant = texture_quadrant;
	lifetime = life;
	gl_FrontColor = gl_Color;
}