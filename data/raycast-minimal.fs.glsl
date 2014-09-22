// inputs
uniform sampler1D opacitytex;
uniform sampler2D fronttex;
uniform sampler2D backtex;
uniform sampler3D luttex;

varying vec4 color;
varying vec2 tc;

// stepsize for ray traversal
const float stepsize = 0.001;

void main(void)
{
	// ray start & end position
	vec3 ray_in  = texture2D( fronttex, tc ).xyz;
	vec3 ray_out = texture2D( backtex , tc ).xyz;

	// ray direction and traversal length
	vec3  dir = ray_out - ray_in;
	float len = length(dir);    // total length
		  dir = dir / len;      // normalize direction
	
	// traversal step vector
	vec3 step = stepsize * dir;

	// initial ray position
	vec3 ray = vec3(0,0,0);

	// initial accumulated color and opacity
	vec4 dst = vec4(0,0,0,0);
	
	int numSteps = int(1/stepsize)+1;

	// ray traversal
	for( int i=0; i < numSteps; ++i )
	{
		// ray termination
		if( length(ray) >= len || dst.a >= 0.99 ) break;

		// transfer function
		
		vec4 src = texture3D( luttex, ray_in + ray );
			// Note: Above should really be done with exponential!
                //src.a = 0.01;
		vec3 color_red = vec3(1,0,0);
		vec3 color_green = vec3(0,1,0);
		vec3 color_blue = vec3(0,0,1);
		vec3 color_yellow = vec3(1,1,0);
		
		float opacity_red = texture1D(opacitytex,0).a;
		float opacity_green = texture1D(opacitytex,0.25).a;
		float opacity_blue = texture1D(opacitytex,0.5).a;
		float opacity_yellow =texture1D(opacitytex,1).a;
		
		color_red = color_red * opacity_red;
		color_green = color_green*opacity_green;
		color_blue = color_blue*opacity_blue;
		color_yellow = color_yellow*opacity_yellow;
		
		
		vec3 p_color_red = color_red * src.r;
		vec3 p_color_green = color_green *src.g;
		vec3 p_color_blue = color_blue*src.b;
		vec3 p_color_yellow = color_yellow*src.a;
		
		float p_opacity_red = opacity_red*src.r;
		float p_opacity_green = opacity_green*src.g;
		float p_opacity_blue = opacity_blue*src.b;
		float p_opacity_yellow = opacity_yellow*src.a;
		
		vec3 sum = p_color_red + p_color_green + p_color_blue + p_color_yellow;
		float opacity_sum = p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow;
		 if(opacity_sum!=0)
			sum =sum/opacity_sum;
		 else
			 sum = vec3(0,0,0);
		vec4 final_color = vec4(sum.r,sum.g,sum.b,p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow);
		
		
		// front-to-back compositing
		dst = dst + (1.0 - dst.a) * final_color;

		// advance ray position
		ray += step;
	}

	gl_FragColor = dst;
	
	// Uncomment the following code to show transfer function for debugging
	//if( tc.y > 0.9 )
	//	gl_FragColor = vec4( texture1D( luttex, tc.x ) ) / (5.0*stepsize);
	//if( tc.y > 0.95 )
	//	gl_FragColor = vec4( - log(-texture1D( luttex, tc.x ).a + 1.0) / stepsize );
	//gl_FragColor = vec4(color.rgb * len * 0.68, 1); // DEBUG ray length (scale by 1/sqrt(3))
}
