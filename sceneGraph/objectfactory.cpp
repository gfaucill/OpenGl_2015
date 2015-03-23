#include "objectfactory.h"
#include "../tools/basic_mesh.hpp"

ObjectFactory::ObjectFactory()
{
    sphere = STP3D::meshSphere(40, 40);
    sphere->createVAO();
    sphere->releaseCPUMemory();

    cylinder = STP3D::meshCylinder(40,40);
    cylinder->createVAO();
    cylinder->releaseCPUMemory();

    cone = STP3D::meshCone(40,40,1);
    cone->createVAO();
    cone->releaseCPUMemory();

    mobius = STP3D::meshMobius(40,40,1);
    mobius->createVAO();
    mobius->releaseCPUMemory();

    squareDoughnut = STP3D::meshSquareDoughnut(40,40);
    squareDoughnut->createVAO();
    squareDoughnut->releaseCPUMemory();

    tore = STP3D::meshTore(40,40,2);
    tore->createVAO();
    tore->releaseCPUMemory();

    cube = STP3D::meshCube(40,40);
    cube->createVAO();
    cube->releaseCPUMemory();
}

StandardObject *ObjectFactory::createSphere(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(sphere);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createCylinder(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(cylinder);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createCone(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(cone);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createMobius(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(mobius);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createSquareDoughnut(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(squareDoughnut);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createTore(ObjMaterial mat)
{
    StandardObject * o = new StandardObject(tore);
    o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
    return o;
}

StandardObject *ObjectFactory::createCube(ObjMaterial mat)
{
	StandardObject * o = new StandardObject(cube);
	o->setMaterials(mat.color, mat.ambi,mat.diff, mat.spec, mat.shine);
	return o;
}
