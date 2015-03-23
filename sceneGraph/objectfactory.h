#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "standardObject.h"
#include "../tools/basic_mesh.hpp"
#include "tree.hpp"

/**
 * @brief The ObjectFactory class
 *  This class store all basic objects
 */
class ObjectFactory
{
public:
    ObjectFactory();

    StandardObject* createSphere(ObjMaterial mat);
    StandardObject* createCylinder(ObjMaterial mat);
    StandardObject* createCone(ObjMaterial mat);
    StandardObject* createMobius(ObjMaterial mat);
    StandardObject* createSquareDoughnut(ObjMaterial mat);
    StandardObject* createTore(ObjMaterial mat);
    StandardObject* createCube(ObjMaterial mat);

private:
     STP3D::IndexedMesh *sphere, *cylinder, *cone, *mobius, *squareDoughnut, *tore, *cube;

};

#endif // OBJECTFACTORY_H
