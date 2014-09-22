// Max Hermann, March 22, 2010
#include "RaycastShader.h"
#include <string>
#include <sstream>
#include <cassert>

#include "glm-0.9.5.4\glm\glm\glm.hpp"
#include "glm-0.9.5.4\glm\glm\ext.hpp"
#include  "glm-0.9.5.4\glm\glm\gtc\matrix_transform.hpp"

using namespace GL;
using namespace std;

bool RaycastShader::diffuseSwitch()
{	
	
		shadingOn.b = 0;
		return checkGLError( "RaycastShader::diffuseSwitch()" );
	
}
//------------------------------------------------------------------------------
bool RaycastShader::init()
{
	// release previous shader
	if( m_shader ) deinit();

	// create shader
	m_shader = new GLSLProgram;
	if( !m_shader )
	{
		cerr << "Error: Creation of Raycast GLSL shader failed!" << endl;
		return false;
	}

	//// Load default set of shaders
	//if( !load_shader( "shader/raycast.vs.glsl", "shader/raycast.fs.glsl" ) )
	//	return false;

	return checkGLError( "RaycastShader::init()" );
}

//------------------------------------------------------------------------------
bool RaycastShader::load_shader( const char* vertex_shader, const char* fragment_shader )
{
	release();

	// load & compile
	if( !m_shader->load_from_disk( vertex_shader, fragment_shader ) )
	{
		cerr << "Error: Compilation of Raycast GLSL shader failed!" << endl;
		return false;
	}

	// get uniform locations
	m_loc_opacity = m_shader->getUniformLocation("opacitytex");
	//m_loc_voltex   = m_shader->getUniformLocation( "voltex"   );
	m_loc_ambientMaterial =m_shader->getUniformLocation("uAmbientMaterial");
	m_loc_diffuseMaterial =m_shader->getUniformLocation("uDiffuseMaterial");
	m_loc_specularMaterial =m_shader->getUniformLocation("uSpecularMaterial");
	m_loc_model = m_shader->getUniformLocation("model");
	m_loc_lightColor = m_shader->getUniformLocation( "uLightColor");
	m_loc_fronttex = m_shader->getUniformLocation( "fronttex" );
	m_loc_backtex  = m_shader->getUniformLocation( "backtex"  );
	m_loc_luttex   = m_shader->getUniformLocation( "luttex"  );
	m_loc_isovalue = m_shader->getUniformLocation( "isovalue" );
	m_loc_voxelsize= m_shader->getUniformLocation( "voxelsize" );
	m_loc_lightPosition = m_shader->getUniformLocation("uLightPosition");
	m_loc_specularMaterial = m_shader->getUniformLocation("uSpecularMaterial");
	m_loc_specularityExponent = m_shader->getUniformLocation( "uSpecularityExponent");
	m_loc_shadingSwitches = m_shader->getUniformLocation( " shadingSwitch" );
	return checkGLError( "RaycastShader::load_shader()" );

	
}

//------------------------------------------------------------------------------
void RaycastShader::deinit()
{
	if(m_shader) delete m_shader; m_shader=NULL;
}

//------------------------------------------------------------------------------
void RaycastShader::set_voxelsize( float sx, float sy, float sz )
{
	m_voxelsize[0] = sx;
	m_voxelsize[1] = sy;
	m_voxelsize[2] = sz;
}

//------------------------------------------------------------------------------
void RaycastShader::bind( GLuint opacitytex, GLuint fronttex, GLuint backtex, GLuint luttex )
{
	if( !m_shader ) return;

	// bind shader
	m_shader->bind();
	//glUniform1i( m_loc_voltex  , voltex   );
	glUniform1i( m_loc_opacity, opacitytex);
	glUniform1i( m_loc_fronttex, fronttex );
	glUniform1i( m_loc_backtex , backtex  );
	glUniform1i( m_loc_luttex  , luttex  );
	glUniform1f( m_loc_isovalue, m_isovalue );
	glUniform3fv( m_loc_voxelsize, 1, m_voxelsize );
	const glm::vec3 eyePos = glm::vec3(0.0f, 3.0f, 5.0f);
	glm::mat4 ModelViewMatrix = glm::lookAt(eyePos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	const glm::mat3 mAmbientMaterial =
        glm::mat3(0.2f, 0.0f, 0.0f,
                  0.0f, 0.2f, 0.0f,
                  0.0f, 0.0f, 0.2f);
	const glm::mat3 mDiffuseMaterial =
        glm::mat3(0.7f, 0.0f, 0.0f,
                  0.0f, 0.7f, 0.0f,
                  0.0f, 0.0f, 0.7f);
	const glm::mat3 mSpecularMaterial =
        glm::mat3(0.95f, 0.0f, 0.0f,
                  0.0f, 0.95f, 0.0f,
                  0.0f, 0.0f, 0.95f);

const float mSpecularityExponent = 40.0f;
	const glm::vec3 vLightColor1 = glm::vec3(1.0f, 1.0f, 1.0f);
	shadingOn = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 vLightPosition1 = glm::vec3(10.0f, 1.0f, 9.0f);
	glUniform3fv(m_loc_lightPosition, 1, glm::value_ptr(vLightPosition1));
	glUniformMatrix4fv(m_loc_model, 1, GL_FALSE,glm::value_ptr(ModelViewMatrix));
	glUniformMatrix3fv(m_loc_ambientMaterial, 1, GL_FALSE, glm::value_ptr(mAmbientMaterial));
	glUniformMatrix3fv(m_loc_diffuseMaterial, 1, GL_FALSE, glm::value_ptr(mDiffuseMaterial));
	 glUniform3fv(m_loc_lightColor, 1, glm::value_ptr(vLightColor1));
	 glUniform3fv(m_loc_shadingSwitches, 1, glm::value_ptr(shadingOn));
	   glUniformMatrix3fv(m_loc_specularMaterial, 1, GL_FALSE, glm::value_ptr(mSpecularMaterial));
    glUniform1f(m_loc_specularityExponent, mSpecularityExponent);

	checkGLError( "RaycastShader::bind()" );
}

//------------------------------------------------------------------------------
void RaycastShader::release()
{
	if( !m_shader ) return;
	m_shader->release();
}

