// inputs
uniform sampler1D opacitytex;
uniform sampler2D fronttex;
uniform sampler2D backtex;
uniform sampler3D luttex;
uniform mat3 uAmbientMaterial;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform mat3 uDiffuseMaterial;
uniform mat4 model;
varying vec3 anormal;
varying vec4 aposition;
varying vec4 color;
varying vec2 tc;
uniform vec3 shadingSwitch;

uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;

// stepsize for ray traversal
const float stepsize = 0.001;
/*vec4 ambient (vec4(final_Color))
{

    return uLightColor * final_Color;
}*/

float shading_diffuse( vec3 L, vec3 N )
{
	return max(dot(L,N),0.0); // TODO: Add specular term
}

float shading_specular(vec3 L, vec3 N)
{
   vec3 bisector = normalize(normalize(vec3(aposition)) - L);
   float cosAngle = max(dot(bisector, normalize(N)), 0);
	return pow(cosAngle, 90.0f)*uDiffuseMaterial;
	
}
/*vec3 diffuse() {

    vec3 pl = normalize(uLightPosition - vec3(aposition));
    float cosAngle = max(dot(pl, normalize(normal_vector), 0);
	return uLightColor * final_color * cosAngle;
    //return uLightColor * uDiffuseMaterial * cosAngle;
}

vec3 specular() {

    vec3 bisector = normalize(normalize(-vec3(aposition)) + normalize((uLightPosition - vec3(aposition))));

    float cosAngle = max(dot(bisector, normalize(normal_vector)), 0);
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
float maximum(float x, float y, float z, float a) {
	float max = x; /* assume x is the largest */

	if (y > max) { /* if y is larger than max, assign y to max */
		max = y;
	} /* end if */

	if (z > max) { /* if z is larger than max, assign z to max */
		max = z;
	} /* end if */
        if (a > max) { /* if z is larger than max, assign z to max */
		max = a;
	}
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
		
		
		mat4 jacobian_matrix;
		jacobian_matrix[0][0] = (src_next_x.r - src_pre_x.r)/2;
		jacobian_matrix[1][0] = (src_next_y.r - src_pre_y.r)/2;
		jacobian_matrix[2][0] = (src_next_z.r - src_pre_z.r)/2;
		jacobian_matrix[3][0] = 0;
		
		jacobian_matrix[0][1] = (src_next_x.g - src_pre_x.g)/2;
		jacobian_matrix[1][1] = (src_next_y.g - src_pre_y.g)/2;
		jacobian_matrix[2][1] = (src_next_z.g - src_pre_z.g)/2;
		jacobian_matrix[3][1] = 0;
		
		jacobian_matrix[0][2] = (src_next_x.b - src_pre_x.b)/2;
		jacobian_matrix[1][2] = (src_next_y.b - src_pre_y.b)/2;
		jacobian_matrix[2][2] = (src_next_z.b - src_pre_z.b)/2;
		jacobian_matrix[3][2] = 0;
				
		jacobian_matrix[0][3] = (src_next_x.a - src_pre_x.a)/2;
		jacobian_matrix[1][3] = (src_next_y.a - src_pre_y.a)/2;
		jacobian_matrix[2][3] = (src_next_z.a - src_pre_z.a)/2;
		jacobian_matrix[3][3] = 0;
			 
		
                //src.a = 0.01;
		vec3 color_red = vec3(1,0,0);
		vec3 color_green = vec3(0,1,0);
		vec3 color_blue = vec3(0,0,1);
		vec3 color_yellow = vec3(1,1,0);
		
		float opacity_red =  texture1D(opacitytex,0).a;
		float opacity_green = texture1D(opacitytex,0.25).a;
		float opacity_blue = texture1D(opacitytex,0.5).a;
		float opacity_yellow =texture1D(opacitytex,1).a;
		
		vec4 weighted_Opacity = vec4(opacity_red *src.r, opacity_green*src.g, opacity_blue*src.b, opacity_yellow*src.a);
	
		vec4 gradient_normal = weighted_Opacity * (jacobian_matrix);
		//gradient_normal = gradient_normal*inverse(gl_ModelViewMatrix);
		vec3 normal_vector =vec3(gradient_normal.x,gradient_normal.y,gradient_normal.z);
		vec4 isosrc = vec4(0,0,0,0);
		float max = maximum(src.r , src.g, src.b, src.a);
		if(src.r == max)
			isosrc.r = src.r;
		else
			isosrc.r = 0;
		
		if(src.g == max)
			isosrc.g = src.g;
		else
			isosrc.g = 0;
		if(src.b == max)
			isosrc.b = src.b;
		else
			isosrc.b = 0;
		if(src.a == max)
			isosrc.a = src.a;
		else
			isosrc.a = 0;
		
		
		vec3 p_color_red = color_red * isosrc.r * opacity_red;
		vec3 p_color_green = color_green *isosrc.g * opacity_green;
		vec3 p_color_blue = color_blue*isosrc.b * opacity_blue;
		vec3 p_color_yellow = color_yellow*isosrc.a * opacity_yellow;
		
		float p_opacity_red = opacity_red*isosrc.r;
		float p_opacity_green = opacity_green*isosrc.g;
		float p_opacity_blue = opacity_blue*isosrc.b;
		float p_opacity_yellow = opacity_yellow*isosrc.a;
		
		vec3 sum = p_color_red + p_color_green + p_color_blue + p_color_yellow;
		float opacity_sum = p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow;
		
		/*if(opacity_sum!=0.0)
			sum =sum/opacity_sum;
		 else
			 sum = vec3(0,0,0);
		*/
		 vec4 final_color = vec4(sum.r,sum.g,sum.b,opacity_sum);
		
		
		// front-to-back compositing
		// if(opacity_sum!=0)
		//dst = dst + (1.0 - dst.a) * final_color + ambient() + diffuse() + specular();
		vec3 lightDirection = normalize(uLightPosition - vec3(aposition));
		 float sv = shading_diffuse( normalize(normal_vector), lightDirection  ) + shading_specular( normalize(normal_vector), lightDirection);
		dst = dst + (1.0 - dst.a) * (vec4( sv*vec3(final_color), final_color.a) + final_color);
		//dst = dst + (1.0 - dst.a) * (shading_diffuse( normalize(normal_vector), lightDirection  ) * weighted_Opacity + (uLightColor*weighted_Opacity) + shading_specular( normalize(normal_vector), lightDirection  ) * weighted_Opacity); 
		//dst = dst + (1.0 - dst.a) * (shading_diffuse( normalize(normal_vector), lightDirection  ) * final_color + (uLightColor*final_color) + shading_specular( normalize(normal_vector), lightDirection  ) * final_color); 
		//dst = dst + (1.0 - dst.a) * (shading_diffuse( normalize(normal_vector), lightDirection ) + shading_specular( normalize(normal_vector), lightDirection)) * final_color; //final_color;
		 
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
