#include "cubemaptexture.h"


CubemapTexture::CubemapTexture(const char *PosXFilename,
                       const char *NegXFilename,
                       const char *PosYFilename,
                       const char *NegYFilename,
                       const char *PosZFilename,
                       const char *NegZFilename)
{

    tex[0] = new STP3D::Texture2D(PosXFilename);
    tex[1] = new STP3D::Texture2D(NegXFilename);
    tex[2] = new STP3D::Texture2D(PosYFilename);
    tex[3] = new STP3D::Texture2D(NegYFilename);
    tex[4] = new STP3D::Texture2D(PosZFilename);
    tex[5] = new STP3D::Texture2D(NegZFilename);
}


void CubemapTexture::initTexture()
{

    glEnable(GL_TEXTURE_CUBE_MAP);

    GLenum types[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    glGenTextures(1, &gl_id_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_id_tex);

//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    for (int i = 0; i < 6; i++ ) {
        if (tex[i]->typetext==TEX_TYPE_RVB) {
           glTexImage2D(types[i], 0, GL_RGB, tex[i]->tex_w, tex[i]->tex_h, 0, GL_RGB,
                         GL_UNSIGNED_BYTE,tex[i]->getTab());
        }
        else if (tex[i]->typetext==TEX_TYPE_RVBA) {
            glTexImage2D(types[i], 0, GL_RGBA, tex[i]->tex_w, tex[i]->tex_h, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE,tex[i]->getTab());
        }
        else if (tex[i]->typetext==TEX_TYPE_LUM){
            glTexImage2D(types[i], 0, GL_RED, tex[i]->tex_w, tex[i]->tex_h, 0, GL_RED,
                         GL_UNSIGNED_BYTE,tex[i]->getTab());
        }
        else {
            STP3D::setError("[Texture : initTexture] NULL initialization of texture is impossible");
        }
        //cout<<"Fin initialisation Texture : "<<*this<<std::endl;
    }

     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

     glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubemapTexture::loadTexture(GLuint target_tex)
{
    last_tex_unit = target_tex;
    //glActiveTextureARB(target_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_id_tex);
}

void CubemapTexture::unloadTexture(GLuint target_tex)
{
    last_tex_unit = target_tex;
    //glActiveTextureARB(last_tex_unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapTexture::~CubemapTexture() {
}
