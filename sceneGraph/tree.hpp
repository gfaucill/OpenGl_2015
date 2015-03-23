/*
 * Tree.hpp
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <algorithm>
#include <list>
#include <stack>

#include "standardObject.h"
#include "../geometry/transformation.hpp"
#include "texture.h"
#include "skyBox.hpp"
#include "../tools/cameraManager.hpp"

/*
 * This class represent a macro object, with they transformation
 * an his base object. It's call tree but it is clearly a directed
 * graph.
 */

class Tree {
public :
    /*
     * Type of node
     */
    typedef enum { TRANSFO, OBJECT, LIGHT, TEXTURE, CAMERA, SKYBOX, NONE } nodeType;


	/*
     * Create a empty root node
	 */
	Tree();
    /*
     * Create a node with data inside (but childs lsit)
     */
    Tree(void * data, nodeType type);

    /**
     * Add a child to this tree
     */
    Tree* add(Tree * t);

    ////////////////////
    ///   Transfo    ///
    ////////////////////
	/**
	 * Add an Hard Transfo to this tree
	 * @return the resulting tree
	 */
	Tree* add(Transfo const& t);
	/**
	 * Link a Transfo to this tree
	 * @return the resulting tree
	 */
	Tree* add(Transfo *t);
	/**
	 * Add a StandardMatrix who represent a transformation
	 */
	Tree* add(StandardMatrix &m);

    ////////////////////
    ///   Object     ///
    ////////////////////
	/**
	 * Add an object (should be the final Node)
	 */
    void add(StandardObject * o);

    ////////////////////
    ///   Light      ///
    ////////////////////
    /**
     * Add a light on the tree
    */
    Tree* add(LightFactory::Light *l);


    ////////////////////
    ///   Texture    ///
    ////////////////////
    /**
     * Add a pre-bind texture
     */
    Tree * add(TextureFactory::Texture * txt);

    ////////////////////
    ///   Sky-box    ///
    ////////////////////
    /*
     * Add a skybox object at root of this tree
     */
    void add(SkyBox * skbx);

    ////////////////////
    ///   Camera     ///
    ////////////////////
    Tree* add(Camera * cam);

	/*
	 * Draw All Objects
	 */
    void draw(LightFactory lights);

    /*
     * Compute lights and cameras positions
     * (Should be call before draw() )
    */
    void step();

private :
    /*
     * List who contains all childs
     */
    std::list<Tree*> *m_childs;

    /*
     * Node type
     */
    nodeType m_type;

    /*
     * Data
    */
    union {
        Transfo *t;
        StandardObject *o;
        LightFactory::Light *l;
        TextureFactory::Texture *txt;
        Camera * cam;
        SkyBox * skbx;
    } m_data;

    /* sub routine used by draw() */
    void __draw__(Transfo t, TextureFactory::Texture *txt, LightFactory lights);
    /* sub routine used by step() */
    void __step__(Transfo t);
    /** data should be a type in the data union */
    Tree * add(void * data, nodeType m_type);
};

#endif /* TREE_HPP_ */
