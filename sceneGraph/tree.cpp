/*
 * Tree.cpp
 */

#include "tree.hpp"

using namespace std;

/******************************************************************************
 *                          CLASS TREE                                        *
 ******************************************************************************/

Tree::Tree() : m_childs(NULL), m_type(NONE) {
    m_data.t = NULL;
}

Tree::Tree(void * data, Tree::nodeType type) : m_childs(NULL) {
    switch(type) {
    case TRANSFO : m_data.t = (Transfo*)data;
        break;
    case OBJECT : m_data.o = (StandardObject*)data;
        break;
    case LIGHT: m_data.l = (LightFactory::Light*)data;
        break;
    case TEXTURE : m_data.txt = (TextureFactory::Texture*)data;
        break;
    case CAMERA : m_data.cam = (Camera*)data;
        break;
    case SKYBOX :m_data.skbx = (SkyBox*)data;
    	break;
    }
    m_type = type;
}

Tree* Tree::add(Tree * t){
    if(m_childs == NULL) {
        m_childs = new list<Tree*>();
    }
    m_childs->push_back(t);
    return t;
}

Tree *Tree::add(TextureFactory::Texture *txt)
{ return add(txt, TEXTURE); }

Tree *Tree::add(void *data, Tree::nodeType type)
{ return this->add(new Tree(data, type)); }

Tree* Tree::add(const Transfo& t)
{ return add(new Transfo(t), TRANSFO);}

Tree* Tree::add(Transfo* t)
{return add(t, TRANSFO); }

Tree* Tree::add(StandardMatrix& m)
{ return add(new Transfo(m), TRANSFO); }

void Tree::add(StandardObject * o)
{ add(o, OBJECT); }

Tree* Tree::add(LightFactory::Light *l)
{ return add(l, LIGHT); }

void Tree::add(SkyBox* skbx)
{ add(skbx, SKYBOX); }

Tree* Tree::add(Camera* cam)
{ return add(cam, CAMERA); }


void Tree::draw(LightFactory lights)
{ Transfo t; __draw__(t, NULL, lights); }

/*
 *  TODO verifiaction chargement et dechargement des textures
*/
void Tree::__draw__(Transfo t, TextureFactory::Texture *txt, LightFactory lights) {
	Transfo tt(t);
    TextureFactory::Texture *ttxt(txt);

    if (m_type == OBJECT) {
        (m_data.o)->draw(tt, lights);
    }
    else {
        switch(m_type) {
			case TRANSFO : tt = tt * *m_data.t;
				break;
			case TEXTURE : /* chargement texture */
				m_data.txt->load();
				ttxt = txt;
				break;
			case SKYBOX :
				ShaderTools::sendModelMatrix(tt.m_md);
				m_data.skbx->load();
					m_data.skbx->draw();
				m_data.skbx->unload();
			break;
        }
        if (m_childs != NULL) {
			for(list<Tree*>::iterator it=m_childs->begin(); it != m_childs->end(); it++) {
				(*it)->__draw__(tt, ttxt, lights);
				if(m_type == TEXTURE) {
					/* dechargement texture */
                    if(m_data.txt != NULL) m_data.txt->unload();
					if (ttxt != NULL) ttxt->load();
				}
			}
        }
    }
}

void Tree::step()
{ Transfo t; __step__(t); }

void Tree::__step__(Transfo t)
{
    Transfo tt(t);

    float m[] = { (float)t.m_md.mat[0], (float)t.m_md.mat[1], (float)t.m_md.mat[2], (float)t.m_md.mat[3],
			(float)t.m_md.mat[4], (float)t.m_md.mat[5], (float)t.m_md.mat[6], (float)t.m_md.mat[7],
			(float)t.m_md.mat[8], (float)t.m_md.mat[9], (float)t.m_md.mat[10], (float)t.m_md.mat[11],
			(float)t.m_md.mat[12], (float)t.m_md.mat[13], (float)t.m_md.mat[14], (float)t.m_md.mat[15] };

    switch(m_type) {
        case TRANSFO : tt = tt * *m_data.t;
        	break;
        case LIGHT : if(m_data.l != NULL) m_data.l->move(t);
            break;
        case CAMERA :
            m_data.cam->move(STP3D::Matrix4D(m));
        break;
    default : break;
    }
    if (m_childs != NULL) {
		for(list<Tree*>::iterator it=m_childs->begin(); it != m_childs->end(); it++) {
			(*it)->__step__(tt);
		}
    }
}
