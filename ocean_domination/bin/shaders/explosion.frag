uniform sampler2D tex;
varying float quadrant;
varying float lifetime;

void main()
{
	vec2 coord = gl_TexCoord[0].st;

	if(quadrant <= 1.0){
		coord.x = (coord.x * 0.5);
		coord.y = (coord.y * 0.5) + 0.5;
	}
	else if(quadrant <= 2.0){
		coord.x = (coord.x * 0.5) + 0.5;
		coord.y = (coord.y * 0.5) + 0.5;
	}
	else if(quadrant <= 3.0){
		coord.x = (coord.x * 0.5);
		coord.y = (coord.y * 0.5);
	}
	else{
		coord.x = (coord.x * 0.5) + 0.5;
		coord.y = (coord.y * 0.5);
	}

	vec4 texel = texture2D(tex,coord);	
	if(lifetime > 0.0){
		gl_FragColor = texel * gl_Color;
	}
}