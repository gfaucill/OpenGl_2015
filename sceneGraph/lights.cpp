using namespace std;

#include <string>
#include <sstream>
#include "../tools/gl_tools.hpp"
#include "lights.hpp"

LightFactory::LightFactory()
{
	enableLigthing();
}

LightFactory::Light *LightFactory::createLight(STP3D::Vector3D ambient, STP3D::Vector3D diffuse,
    STP3D::Vector3D specular, STP3D::Vector3D position, STP3D::Vector3D direction, float spotCutoff,
    LightType type, LightFactory::ShadingModel shadingModel)
{
    STP3D::Vector4D _ambiant; _ambiant.fromDirection(ambient);
    STP3D::Vector4D _diffuse; _diffuse.fromDirection(diffuse);
    STP3D::Vector4D _specular; _specular.fromDirection(specular);
    STP3D::Vector4D _position; _position.fromPoint(position);
    STP3D::Vector4D _direction; _direction.fromDirection(direction);

    LightParameters lp = {
        _ambiant, _diffuse , _specular, _position, _position,
        _direction, _direction, spotCutoff, shadingModel, type };

    lights.push_back(lp);
    return new LightFactory::Light(&lights.back());
}

LightFactory::Light* LightFactory::createPonctualLight(STP3D::Vector3D ambient,
    STP3D::Vector3D diffuse, STP3D::Vector3D specular,
    STP3D::Vector3D position, LightFactory::ShadingModel shadingModel)
{
    return createLight(ambient, diffuse, specular, position, STP3D::Vector3D(), 0, Ponctual, shadingModel);
}

LightFactory::Light *LightFactory::createDirectionalLight(STP3D::Vector3D ambient,
    STP3D::Vector3D diffuse, STP3D::Vector3D specular, STP3D::Vector3D direction,
    LightFactory::ShadingModel shadingModel)
{
    return createLight(ambient, diffuse, specular, STP3D::Vector3D(), direction, 0, Directional, shadingModel);
}

LightFactory::Light *LightFactory::createSpotLight(STP3D::Vector3D ambient,
    STP3D::Vector3D diffuse, STP3D::Vector3D specular, STP3D::Vector3D direction,
    STP3D::Vector3D position, float spotCutoff, LightFactory::ShadingModel shadingModel)
{
    return createLight(ambient, diffuse, specular, position, direction, spotCutoff, Spot, shadingModel);
}

void LightFactory::sendLight(uint shaderId)
{
	applyLightState();
    stringstream ss;

    // Number of lights
    glUniform1i(glGetUniformLocation(shaderId, "maxLights"), lights.size());

    //send all ligths
    for (uint i= 0 ; i < lights.size(); i++) {
        ss << "Lights[" << i << "].ambient";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].ambient);

        ss.str(""); ss << "Lights[" << i << "].diffuse";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].diffuse);

        ss.str(""); ss << "Lights[" << i << "].specular";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].specular);

        ss.str(""); ss << "Lights[" << i << "].position";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].position);

        ss.str(""); ss << "Lights[" << i << "].spotDirection";
        glUniform3f(glGetUniformLocation(shaderId,ss.str().c_str()), lights[i].direction.x, lights[i].direction.y, lights[i].direction.z );

        ss.str(""); ss << "Lights[" << i << "].spotCutoff";
        glUniform1f(glGetUniformLocation(shaderId,ss.str().c_str()), lights[i].spotCutoff);

        ss.str(""); ss << "Lights[" << i << "].shadingModel";
        glUniform1i(glGetUniformLocation(shaderId,ss.str().c_str()), lights[i].shadingModel);

        ss.str(""); ss << "Lights[" << i << "].lightType";
        glUniform1i(glGetUniformLocation(shaderId,ss.str().c_str()), lights[i].lightType);
    }
    CHECK_GL;
}

void LightFactory::setMaterials(uint shaderId, STP3D::Vector4D color, float ambi, float diff, float spec, float shine)
{
	STP3D::Vector4D ambient;
	STP3D::Vector4D diffuse;
	STP3D::Vector4D specular;

    if(lightON == true) {
		ambient = color * ambi;
		diffuse = color * diff;
		specular = color * spec;
    }
    else {
    	 glBegin(GL_LINES);
    	    glColor3f(color.x, color.y, color.z);
    	    glVertex3f(0.0,0.0,0.0); glVertex3f(0.0,0.0,0.0);
    	 glEnd();
    }
    glUniform4fv(glGetUniformLocation(shaderId, "Materials.ambient"),1, ambient);
	glUniform4fv(glGetUniformLocation(shaderId, "Materials.diffuse"),1, diffuse);
	glUniform4fv(glGetUniformLocation(shaderId, "Materials.specular"),1, specular);
	glUniform1f(glGetUniformLocation(shaderId, "Materials.shininess"), shine);

	updateProduct(shaderId, ambient, diffuse, specular);
}

void LightFactory::updateProduct(uint shaderId, STP3D::Vector4D ambient, STP3D::Vector4D diffuse, STP3D::Vector4D specular)
{
    stringstream ss;

    for (uint i= 0 ; i < lights.size(); i++) {
        ss << "u_lightProduct[" << i << "].ambient";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].ambient ^ ambient);

        ss.str("");  ss << "u_lightProduct[" << i << "].diffuse";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].diffuse ^ diffuse);

        ss.str("");  ss << "u_lightProduct[" << i << "].specular";
        glUniform4fv(glGetUniformLocation(shaderId,ss.str().c_str()), 1, lights[i].specular ^ specular);
    }
}

#define vector3DToVector(vector, vector3D) vector.coord[0] = vector3D.x; vector.coord[1] = vector3D.y; vector.coord[2] = vector3D.z;
#define vectorToVector3D(vector3D, vector) vector3D.x = vector.coord[0]; vector3D.y = vector.coord[1]; vector3D.z = vector.coord[2];


/*
 *  TODO CHECK transfo [switch]
 * */
void LightFactory::Light::move(Transfo &t)
{
    Vector v;
    switch (m_lp->lightType) {
    case Ponctual:
        vector3DToVector(v, m_lp->init_position); v = t.m_md * v; vectorToVector3D(m_lp->position, v);
        break;

    case Directional:
        vector3DToVector(v, m_lp->init_position); v = t.m_md * v; vectorToVector3D(m_lp->position, v);
        vector3DToVector(v, m_lp->init_direction); v = t.m_md * v; vectorToVector3D(m_lp->direction, v);
        break;

    case Spot :
        vector3DToVector(v, m_lp->init_position); v = t.m_md * v; vectorToVector3D(m_lp->position, v);
        vector3DToVector(v, m_lp->init_direction); v = t.m_md * v; vectorToVector3D(m_lp->direction, v);
        break;

    default:
        break;
    }
}

void LightFactory::enableLigthing() {
	int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	if (id != 0)
		glUniform1i(glGetUniformLocation(id,"lightON"), 1);
	lightON = true;
}

void LightFactory::disableLighting() {
	int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	if (id != 0)
		glUniform1i(glGetUniformLocation(id,"lightON"), 0);
	lightON = false;
}

void LightFactory::applyLightState() {
	if(lightON == true)
		enableLigthing();
	else
		disableLighting();
}
