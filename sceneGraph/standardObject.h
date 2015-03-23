#ifndef STANDARDOBJECT_H
#define STANDARDOBJECT_H

#include <GL/glew.h>
#include "../geometry/transformation.hpp"
#include "../tools/vector3d.hpp"
#include "lights.hpp"
#include "../tools/shader.hpp"
#include "../tools/indexed_mesh.hpp"

typedef struct {
    STP3D::Vector4D color;
    float ambi;
    float diff;
    float spec;
    float shine;
}ObjMaterial;

/**
 * TODO : permettre de baser un objet sur plusieur resolutions
 *  afin de pouvoir choisir une resolution adapter lors du dessin
 *
 *  AddIndexedMesh( IndexedMesh * im, int divHeight, int divWidth ) ;
 *
 * ??????????????????????????????????????????????????????????????????
 */
class StandardObject {

public :
    /**
     * @brief StandardObject Construct a standard object based on a indexed mesh
     *  basicly the object are totaly wihte
     * @param Im a indexed mesh
     */
    StandardObject(STP3D::IndexedMesh *Im) : obj(Im) {
        mat.color = STP3D::Vector4D(1,1,1,1);
        mat.ambi =  mat.diff = mat.spec = mat.shine = 1;
    }

    inline void setMaterials(STP3D::Vector4D color, float ambi, float diff, float spec, float shine) {
        mat.color = color;
        mat.ambi = ambi;
        mat.diff = diff;
        mat.spec = spec;
        mat.shine = shine;
    }

    inline void draw(Transfo &t, LightFactory light) {
        /*Matrix*/
        ShaderTools::sendModelMatrix(t.m_md.mat);
        ShaderTools::sendNormalMatrix(t.m_mn.mat);
        /*Materials*/
        int id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);
        light.setMaterials(id, mat.color, mat.ambi, mat.diff, mat.spec, mat.shine);
        /*Draw*/
        obj->draw();
    }

private :
    STP3D::IndexedMesh *obj;
    ObjMaterial mat;
};

#endif // STANDARDOBJECT_H
