// inputs
uniform sampler3D voltex;
uniform sampler2D fronttex;
uniform sampler2D backtex;
uniform sampler1D luttex;

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
		float scalar = texture3D( voltex, ray_in + ray ).w;
		vec4 src = texture1D( luttex, scalar );
			// Note: Above should really be done with exponential!

		// front-to-back compositing
		dst = dst + (1.0 - dst.a) * src;

		// advance ray position
		ray += step;
	}

	gl_FragColor = dst;
	
	// Uncomment the following code to show transfer function for debugging
	if( tc.y > 0.9 )
		gl_FragColor = vec4( texture1D( luttex, tc.x ) ) / (5.0*stepsize);
	if( tc.y > 0.95 )
		gl_FragColor = vec4( - log(-texture1D( luttex, tc.x ).a + 1.0) / stepsize );
	//gl_FragColor = vec4(color.rgb * len * 0.68, 1); // DEBUG ray length (scale by 1/sqrt(3))
}
