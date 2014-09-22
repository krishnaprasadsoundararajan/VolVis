// Max Hermann, March 22, 2010
#ifndef RAYCASTSHADER_H
#define RAYCASTSHADER_H

#include "GLSLProgram.h"
#include "GLError.h"
#include "glm-0.9.5.4\glm\glm\glm.hpp"
#include "glm-0.9.5.4\glm\glm\ext.hpp"
#include  "glm-0.9.5.4\glm\glm\gtc\matrix_transform.hpp"

/// One pass raycasting shader (bundles GLSL program and uniform parameters)
class RaycastShader
{
public:
	RaycastShader()
		: m_shader(NULL),
		  m_isovalue(0.2f)		  
		{}
	~RaycastShader() { if(m_shader) delete m_shader; }

	bool init();
	void deinit(); // destruction of shader only possible in valid GL context

	void bind(GLuint opacitytex, GLuint fronttex, GLuint backtex, GLuint luttex );
	void release();

	bool load_shader( const char* vertex_shader, const char* fragment_shader );

	void  set_isovalue( float iso ) { m_isovalue = iso; }
	float get_isovalue() const { return m_isovalue; }

	void set_voxelsize( float sx, float sy, float sz );
	bool diffuseSwitch();

private:
	GL::GLSLProgram*  m_shader;
	GLint             m_loc_voltex,
	                  m_loc_fronttex,
	                  m_loc_backtex,
					  m_loc_luttex,
					  m_loc_isovalue,
					  m_loc_voxelsize,
					  m_loc_model,
					  m_loc_opacity,
					  m_loc_ambientMaterial,
					  m_loc_diffuseMaterial,
					  m_loc_lightColor,
					  m_loc_lightPosition,
					  m_loc_specularMaterial,
					  m_loc_specularityExponent,
					  m_loc_shadingSwitches;
	float m_isovalue;
	glm::vec3 shadingOn;
	glm::mat3 mDiffuseMaterial;
	float m_voxelsize[3];
};

#endif
