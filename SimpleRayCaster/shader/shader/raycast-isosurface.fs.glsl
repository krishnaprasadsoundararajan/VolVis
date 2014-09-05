// raycast - fragment shader
//
// Straight forward single-pass raycasting. Shader is initialized with
// traversal start and end positions in "fronttex" and "backtex". 
// In the easiest case you just render the front- respectively back-faces 
// of the volume bounding box to the front- and back-textures with canonical
// RGB coloring from (0,0,0) to (1,1,1).
//
// Max Hermann, March 22, 2010

// Debug modes:
// 1 = debug ray termination (red=left volume, green=opacity>=threshold)
// 2 = render ray length
// 3 = render texture coordinate
//#define DEBUG

// Non-polygonal isosurface rendering with Phong shading
// and intersection refinement
#define ISOSURFACE  

// Lighting for direct volume rendering (still experimental)
//#define LIGHTING

// input
uniform sampler3D voltex;
uniform sampler2D fronttex;
uniform sampler2D backtex;
uniform sampler1D luttex;
varying vec4 color;
varying vec2 tc;

uniform float isovalue;

// size of a voxel (for finite differences)  FIXME: should be set by application
uniform vec3 voxelsize; // = vec3( 1.0/128.0, 1.0/128.0, 1.0/256.0 );
// non-polygonal isosurface                  FIXME: should be a user parameter
//const float isovalue = 0.99; //0.042;
// stepsize for ray traversal                FIXME: should be a user parameter
const float stepsize = 0.0005; // 0.001
// number of isosurface refinement steps
const int refinement_steps = 5;

// light/material definition                 FIXME: should be adjustable (use GL_LIGHT0?)
const vec3  light_pos = vec3(0.0,-1.0,0.0); // light position (in volume coordinates)
const vec3 mat_ka  = 0.1 * vec3(1.0,1.0,1.0);   // ambient  coeff. w/ color
const vec3 mat_ks  = 0.3 * vec3(1.0,1.0,1.0);   // specular coeff. w/ color
const vec3 mat_kd  = 0.6 * vec3(0.7,0.7,1.0);   // diffuse  coeff. w/ color
const float mat_exp = 16.0;  // Phong exponent

// Compute normal by central differences
vec3 get_normal( vec3 x )
{		
	vec3 n = vec3( 
	  texture3D(voltex, x - vec3(voxelsize.x,0,0)).w - texture3D(voltex, x + vec3(voxelsize.x,0,0)).w, 
	  texture3D(voltex, x - vec3(0,voxelsize.y,0)).w - texture3D(voltex, x + vec3(0,voxelsize.y,0)).w, 
	  texture3D(voltex, x - vec3(0,0,voxelsize.z)).w - texture3D(voltex, x + vec3(0,0,voxelsize.z)).w
	);
	return normalize(n);
}

// Compute phong illumation (n==normal at x, eye==vector from x to viewer)
// TODO: check computation and constants, just copied from internet ;-)
vec3 phong( vec3 n, vec3 eye )
{
	float I = 1.0;
	vec3 L = normalize( light_pos );
	vec3 E = normalize( eye );
	
	vec3 H = normalize( L + E );
	
	vec3 R = reflect( -L, n );
	float diff = max( dot(L,n), 0.0 );
	float spec = 0.0;
	if( diff > 0.0 )
	{
		//spec = max( dot(R,E), 0.0 ); // Phong
		spec = max( dot(H,n), 0.0 ); // Blinn-Phong
		spec = pow( spec, mat_exp );
	}

	return mat_ka + mat_ks*spec + mat_kd*diff;
}

void main(void)
{
	//vec2 tc = gl_TexCoord[0].st;//NOT WORKING => passing tc now as varying

	// ray start & end position
	vec3 ray_in  = texture2D( fronttex, tc ).xyz;
	vec3 ray_out = texture2D( backtex , tc ).xyz;

	// ray direction and traversal length
	vec3  dir = ray_out - ray_in;
	float len = length(dir);
		  dir = dir / len;      // normalize direction

	// traversal step vector
	vec3 step = stepsize * dir;

	// initial ray position
	vec3 ray = step; //ray_in; // =step ?

	// initial accumulated color and opacity
	vec4 dst = vec4(0,0,0,0);

	// ray traversal
	for( int i=0; i < int(1/stepsize)+1; ++i )
	{
		// ray termination
	#ifndef DEBUG
		if( length(ray) >= len || dst.a >= 0.99 ) break;
	#else			
		if( length(ray) >= len ) 
		{ 
			dst = vec4(1,0,0,1);  // DEBUG break condition
			break; 
		}
		if( dst.a >= 0.99 ) 
		{ 
			dst.a = 1.0; 
			dst = vec4(0,1,0,1);  // DEBUG break condition
			break; 
		}
	#endif

		// TODO: transfer funcion
		float intensity = texture3D( voltex, ray_in + ray ).w;

#ifdef ISOSURFACE
		if( intensity > isovalue )
		{
			// intersection refinement
			float searchdir = -1;    // initial step is backwards
			vec3 ministep = step*.5;
			for( int j=0; j < refinement_steps; ++j )
			{
				// binary search
				ray += searchdir*ministep;
				
				intensity = texture3D( voltex, ray_in+ray );

				// move backwards until exact intersection overstepped
				// and vice versa
				if( intensity > isovalue )
					searchdir = -1;
				else
					searchdir = 1;

				// half stepsize
				ministep *= .5;
			}

			dst.rgb = phong( get_normal(ray_in+ray), -dir );
			dst.a   = 1;
			break;
		}
#else
		vec4 src = intensity;

	#ifdef LIGHTING
		float li = phong( get_normal(ray_in+ray), -dir );
		dst.rgb = (dst.rgb + (1-dst.a))*src.rgb * li;
		dst.a   = (dst.a   + (1-dst.a))*src.a;
	#else
		// front-to-back compositing			
		dst = dst + (1 - dst.a) * src;
	#endif

#endif
		// advance ray position
		ray += step;
	}

	//gl_FragColor = vec4( 0,1,0,1 );
	gl_FragColor = dst;
	//gl_FragColor = vec4(color.rgb * len * 0.68, 1); // DEBUG ray length (scale by 1/sqrt(3))
	//gl_FragColor = vec4(1,tc.s,tc.t,1);             // DEBUG texture coordinates
}
