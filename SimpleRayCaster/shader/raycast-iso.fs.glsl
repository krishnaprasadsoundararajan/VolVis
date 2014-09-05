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
uniform mat3 uSpecularMaterial;
uniform float uSpecularityExponent;
vec3 anormal1;
// stepsize for ray traversal
const float stepsize = 0.001;
vec3 ambient() {

    return uLightColor * uAmbientMaterial;
}

vec3 diffuse() {

    vec3 pl = normalize(uLightPosition - vec3(aposition));
    float cosAngle = max(dot(pl, normalize(anormal1)), 0);

    return uLightColor * uDiffuseMaterial * cosAngle;
}

vec3 specular() {

    vec3 bisector = normalize(normalize(-vec3(aposition)) + normalize((uLightPosition - vec3(aposition))));

    float cosAngle = max(dot(bisector, normalize(anormal1)), 0);

    return uLightColor * uSpecularMaterial * pow(cosAngle, uSpecularityExponent);
}

float spotLight() {

	vec3 ray_in  = texture2D( fronttex, tc ).xyz;
	vec3 ray_out = texture2D( backtex , tc ).xyz;
       vec3  lightDir = ray_out - ray_in;

    vec3 pl = normalize(vec3(aposition) - uLightPosition);

    float cosAngle = max(dot(lightDir, pl), 0);

    return pow(cosAngle, 40.0f);
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
		
		vec4 src = texture3D( luttex, ray_in + ray );
		vec4 src_next_x = vec4(0,0,0,0);
		vec4 src_pre_x = vec4(0,0,0,0);
		vec3 rayPosition = ray_in + ray;
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
		float maximum = (src.r > src.g ? src.r : src.g) > (src.b > src.a ? src.b : src.a) ? (src.r > src.g ? src.r : src.g) : (src.b > src.a ? src.b : src.a);
		if(src.r < 0.5)
			src.r = 0;
		if(src.g < 0.5)
			src.g = 0;
		if(src.b < 0.5)
			src.b = 0;
		if(src.a < 0.5)
			src.a = 0;
		
		vec3 p_color_red = color_red * src.r;
		vec3 p_color_green = color_green *src.g;
		vec3 p_color_blue = color_blue*src.b;
		vec3 p_color_yellow = color_yellow*src.a;
		
		float p_opacity_red = opacity_red*src.r;
		float p_opacity_green = opacity_green*src.g;
		float p_opacity_blue = opacity_blue*src.b;
		float p_opacity_yellow = opacity_yellow*src.a;
		vec4 opacity_gradient = vec4(src.r,src.g,src.b,src.a);
		vec4 gradient_normal =  opacity_gradient * gradient   ;
		anormal1 =vec3(gradient_normal.x,gradient_normal.y,gradient_normal.z);
		anormal1 = normalize(anormal1);
		
		vec3 sum = p_color_red + p_color_green + p_color_blue + p_color_yellow;
		float opacity_sum = p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow;
		 if(opacity_sum!=0)
			sum =sum/opacity_sum;
		 else
			 sum = vec3(0,0,0);
		vec4 final_color = vec4(sum.r,sum.g,sum.b,p_opacity_red + p_opacity_green + p_opacity_blue + p_opacity_yellow);
		
		
		// front-to-back compositing
		//if( opacity_sum > 0.9)
			dst = dst + (1.0 - dst.a) * final_color;

		// advance ray position
		ray += step;
	}

	gl_FragColor = dst  + ambient() + diffuse() + specular();

	
	// Uncomment the following code to show transfer function for debugging
	//if( tc.y > 0.9 )
	//	gl_FragColor = vec4( texture1D( luttex, tc.x ) ) / (5.0*stepsize);
	//if( tc.y > 0.95 )
	//	gl_FragColor = vec4( - log(-texture1D( luttex, tc.x ).a + 1.0) / stepsize );
	//gl_FragColor = vec4(color.rgb * len * 0.68, 1); // DEBUG ray length (scale by 1/sqrt(3))
}