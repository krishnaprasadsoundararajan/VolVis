// inputs
uniform sampler1D opacitytex;
uniform sampler2D fronttex;
uniform sampler2D backtex;
uniform sampler3D luttex;
uniform mat3 uAmbientMaterial;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform mat3 uDiffuseMaterial;
varying vec3 anormal;
varying vec4 aposition;
varying vec4 color;
varying vec2 tc;
vec3 anormal1;
vec4 final_color;
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

// stepsize for ray traversal
const float stepsize = 0.001;
vec3 ambient() {

    return uLightColor * uAmbientMaterial;
}

float shading( vec3 L, vec3 N )
{
	return max(dot(L,N),0.0); // TODO: Add specular term
}

/*vec3 diffuse() {

    vec3 pl = normalize(uLightPosition - vec3(aposition));
    float cosAngle = max(dot(pl, normalize(anormal1), 0);
	return uLightColor * final_color * cosAngle;
    //return uLightColor * uDiffuseMaterial * cosAngle;
}

vec3 specular() {

    vec3 bisector = normalize(normalize(-vec3(aposition)) + normalize((uLightPosition - vec3(aposition))));

    float cosAngle = max(dot(bisector, normalize(anormal1)), 0);
	return uLightColor * final_color * pow(cosAngle, uSpecularityExponent);
    //return uLightColor * uSpecularMaterial * pow(cosAngle, uSpecularityExponent);
}*/

float spotLight() {

	vec3 ray_in  = texture2D( fronttex, tc ).xyz;
	vec3 ray_out = texture2D( backtex , tc ).xyz;
       vec3  lightDir = ray_out - ray_in;

    vec3 pl = normalize(vec3(aposition) - uLightPosition);

    float cosAngle = max(dot(lightDir, pl), 0);

    return pow(cosAngle, 40.0f);
}
float maximum(float x, float y, float z) {
	float max = x; /* assume x is the largest */

	if (y > max) { /* if y is larger than max, assign y to max */
		max = y;
	} /* end if */

	if (z > max) { /* if z is larger than max, assign z to max */
		max = z;
	} /* end if */

	return max; /* max is the largest value */
}
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
		
		// get probability classes
		vec4 src = texture3D( luttex, ray_in + ray );
		
		vec3 rayPosition = ray_in + ray;
		
#if 1 // NO BOUNDARY FOR NOW
		vec3 lutres = vec3( 94.0, 150.0, 150.0 );
		vec3 voxelsize = 1.0 / lutres;
		
		vec4 src_next_x = texture3D( luttex, ray_in + ray + vec3(1.0,0.0,0.0)*voxelsize ); 
		vec4 src_pre_x  = texture3D( luttex, ray_in + ray - vec3(1.0,0.0,0.0)*voxelsize ); 
		vec4 src_next_y = texture3D( luttex, ray_in + ray + vec3(0.0,1.0,0.0)*voxelsize ); 
		vec4 src_pre_y  = texture3D( luttex, ray_in + ray - vec3(0.0,1.0,0.0)*voxelsize ); 
		vec4 src_next_z = texture3D( luttex, ray_in + ray + vec3(0.0,0.0,1.0)*voxelsize ); 
		vec4 src_pre_z  = texture3D( luttex, ray_in + ray - vec3(0.0,0.0,1.0)*voxelsize ); 
		
#else // FIRST ATTEMPT AT BOUNDARY HANDLING
		//  handle boundaries x
		vec4 src_next_x = vec4(0,0,0,0);
		vec4 src_pre_x = vec4(0,0,0,0);
		if( rayPosition.r == 0)
		{
		src_next_x = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x+2,rayPosition.y, rayPosition.z);
		src_pre_x  = texture3D( luttex, temp);
		}
		else if(rayPosition.r  ==94)
		{
		src_next_x = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x-2,rayPosition.y, rayPosition.z);
		src_pre_x  = texture3D( luttex, temp);
		}
		else
		{
		vec3 temp2 = vec3(rayPosition.x-1,rayPosition.y, rayPosition.z);	
		src_next_x = texture3D( luttex, temp2);
		vec3 temp = vec3(rayPosition.x+1,rayPosition.y, rayPosition.z);
		src_pre_x  = texture3D( luttex, temp);
		}
		
		//  handle boundaries y
		vec4 src_next_y = vec4(0,0,0,0);
		vec4 src_pre_y = vec4(0,0,0,0);
		if( rayPosition.g == 0)
		{
		src_next_y = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x,rayPosition.y+2, rayPosition.z);
		src_pre_y  = texture3D( luttex, temp);
		}
		else if(rayPosition.g  ==150)
		{
		src_next_y = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x,rayPosition.y-2, rayPosition.z);
		src_pre_y  = texture3D( luttex, temp);
		}
		else
		{
		vec3 temp2 = vec3(rayPosition.x,rayPosition.y-1, rayPosition.z);	
		src_next_y = texture3D( luttex, temp2);
		vec3 temp = vec3(rayPosition.x,rayPosition.y+1, rayPosition.z);
		src_pre_y  = texture3D( luttex, temp);
		}
		
		//  handle boundaries x
		vec4 src_next_z = vec4(0,0,0,0);
		vec4 src_pre_z = vec4(0,0,0,0);
		if( rayPosition.b == 0)
		{
		src_next_z = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x,rayPosition.y, rayPosition.z+2);
		src_pre_z  = texture3D( luttex, temp);
		}
		else if(rayPosition.b  ==150)
		{
		src_next_z = texture3D( luttex, rayPosition);
		vec3 temp = vec3(rayPosition.x,rayPosition.y, rayPosition.z-2);
		src_pre_z = texture3D( luttex, temp);
		}
		else
		{
		vec3 temp2 = vec3(rayPosition.x,rayPosition.y, rayPosition.z-1);	
		src_next_z = texture3D( luttex, temp2);
		vec3 temp = vec3(rayPosition.x,rayPosition.y, rayPosition.z+1);
		src_pre_z  = texture3D( luttex, temp);
		}
#endif
		
		
		mat4 gradient;
		gradient[0][0] = (src_next_x.r - src_pre_x.r)/2;
		gradient[1][0] = (src_next_y.r - src_pre_y.r)/2;
		gradient[2][0] = (src_next_z.r - src_pre_z.r)/2;
		gradient[3][0] = 0;
		
		gradient[0][1] = (src_next_x.g - src_pre_x.g)/2;
		gradient[1][1] = (src_next_y.g - src_pre_y.g)/2;
		gradient[2][1] = (src_next_z.g - src_pre_z.g)/2;
		gradient[3][1] = 0;
		
		gradient[0][2] = (src_next_x.b - src_pre_x.b)/2;
		gradient[1][2] = (src_next_y.b - src_pre_y.b)/2;
		gradient[2][2] = (src_next_z.b - src_pre_z.b)/2;
		gradient[3][2] = 0;
				
		gradient[0][3] = (src_next_x.a - src_pre_x.a)/2;
		gradient[1][3] = (src_next_y.a - src_pre_y.a)/2;
		gradient[2][3] = (src_next_z.a - src_pre_z.a)/2;
		gradient[3][3] = 0;
			 
		
                //src.a = 0.01;
		vec3 color_red = vec3(1,0,0);
		vec3 color_green = vec3(0,1,0);
		vec3 color_blue = vec3(0,0,1);
		vec3 color_yellow = vec3(1,1,0);
		
		float opacity_red =  0;//texture1D(opacitytex,0).a;
		float opacity_green = 0.1;//texture1D(opacitytex,0.25).a;
		float opacity_blue = 0.1;//texture1D(opacitytex,0.5).a;
		float opacity_yellow =1;//texture1D(opacitytex,1).a;
		
		vec4 weighted_Opacity = vec4(opacity_red *src.r, opacity_green*src.g, opacity_blue*src.b, opacity_yellow*src.a);
	
		vec4 gradient_normal = weighted_Opacity * gradient;
		anormal1 =vec3(gradient_normal.x,gradient_normal.y,gradient_normal.z);
		
		
		//vec4 weighted_colors = vec4(color_red*opacity_red *src.r, color_green*opacity_green*src.g, color_blue*opacity_blue*src.b, color_yellow*opacity_yellow*src.a);
		color_red = color_red * opacity_red;
		color_green = color_green*opacity_green;
		color_blue = color_blue*opacity_blue;
		color_yellow = color_yellow*opacity_yellow;
		
		
		vec3 p_color_red = color_red * src.r;
		vec3 p_color_green = color_green *src.g;
		vec3 p_color_blue = color_blue*src.b;
		vec3 p_color_yellow = color_yellow*src.a;
		
		vec3 sum_weighted_colors = p_color_red + p_color_green + p_color_blue + p_color_yellow;
		
		float p_opacity_red = opacity_red*src.r;
		float p_opacity_green = opacity_green*src.g;
		float p_opacity_blue = opacity_blue*src.b;
		float p_opacity_yellow = opacity_yellow*src.a;
		
		float opacity_sum = p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow;
		final_color = vec4(sum_weighted_colors.r, sum_weighted_colors.g, sum_weighted_colors.b, opacity_sum);
		
		
		
		
#if 0		
		float lamda_w1 = 0;
		
		float src_a=1,src_r=1,src_g=1,src_b=1;
		
		if(src.r != 0)
		    lamda_w1 = maximum( (src.g/src.r) , (src.b/src.r) , (src.a / src.r) );
		else
		    src_r = 0;
		
		float lamda_w2 = 0;
		if(src.g != 0)
			lamda_w2 = maximum( (src.r/src.g) , (src.b/src.g) , (src.a/ src.g) );
		else
			src_g = 0;
		
		float lamda_w3 = 0;
		if( src.b !=0)
			lamda_w3 = maximum( (src.r/src.b) , (src.g/src.b) , (src.a/src.b) );
		src_b =0;
		
			
		float lamda_w4 = 0;
		if( src.a !=0)
			lamda_w4 = maximum( (src.r/src.a) , (src.g/src.a) , (src.b / src.a) );
		else
			src_a = 0;
		
		
		
		if(lamda_w1 != 0)
			src_r = (1/lamda_w1);
		
		
		if(lamda_w2 != 0)
			src_g = (1/lamda_w2);
		
		if(lamda_w3 != 0)
			src_b = (1/lamda_w3);
		
		if(lamda_w4 != 0)
			src_a = (1/lamda_w4);
		
		src_r = src_r / (src_r + src_g + src_b + src_a);
		src_g = src_g / (src_r + src_g + src_b + src_a);
		src_b = src_b / (src_r + src_g + src_b + src_a);
		src_a = src_a / (src_r + src_g + src_b + src_a);
		
		vec3 p_color_red = color_red * src_r;
		vec3 p_color_green = color_green *src_g;
		vec3 p_color_blue = color_blue*src_b;
		vec3 p_color_yellow = color_yellow*src_a;
		
		float p_opacity_red = opacity_red*src_r;
		float p_opacity_green = opacity_green*src_g;
		float p_opacity_blue = opacity_blue*src_b;
		float p_opacity_yellow = opacity_yellow*src_a;
		
		
		vec3 sum = p_color_red + p_color_green + p_color_blue + p_color_yellow;
		float opacity_sum = p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow;
		 if(opacity_sum!=0)
			sum =sum/opacity_sum;
		 else
			 sum = vec3(0,0,0);
		vec4 final_color = vec4(sum.r,sum.g,sum.b,p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow);
		
#endif		
		// front-to-back compositing
		// if(opacity_sum!=0)
		//dst = dst + (1.0 - dst.a) * final_color + ambient() + diffuse() + specular();
		dst = dst + (1.0 - dst.a) * shading( normalize(anormal1), normalize(uLightPosition - vec3(aposition)) ) * final_color; //final_color;
		 
		// advance ray position
		ray += step;
	}

	gl_FragColor = dst ;

	
	// Uncomment the following code to show transfer function for debugging
	//if( tc.y > 0.9 )
	//	gl_FragColor = vec4( texture1D( luttex, tc.x ) ) / (5.0*stepsize);
	//if( tc.y > 0.95 )
	//	gl_FragColor = vec4( - log(-texture1D( luttex, tc.x ).a + 1.0) / stepsize );
	//gl_FragColor = vec4(color.rgb * len * 0.68, 1); // DEBUG ray length (scale by 1/sqrt(3))
}
