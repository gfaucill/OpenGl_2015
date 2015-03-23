#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include "../tools/vector3d.hpp"
#include "../tools/vector4d.hpp"
#include "../geometry/transformation.hpp"
#include <vector>


/**
 * @brief The LightFactory class This class manage all lights in the scene
 * This factory provide NodeLight to put into the scene graph
 */
class LightFactory
{
public:
    class Light;
    typedef enum {Phong = 0} ShadingModel;

private:
    typedef enum {Ponctual, Directional, Spot} LightType;
    /*
     * Generic method to create a light
     */
    Light* createLight(STP3D::Vector3D ambient, STP3D::Vector3D diffuse,
    STP3D::Vector3D specular, STP3D::Vector3D position,
    STP3D::Vector3D direction, float spotCutoff, LightType type,
    ShadingModel shadingModel=Phong);

    /*
     * Compute and send to the shader light products
     */
    void updateProduct(uint shaderId, STP3D::Vector4D ambient, STP3D::Vector4D diffuse, STP3D::Vector4D specular);

    /*
     * Send light state to the shader if is not already done
     */
    void applyLightState();

    typedef struct {
       STP3D::Vector4D ambient;
       STP3D::Vector4D diffuse;
       STP3D::Vector4D specular;
       STP3D::Vector4D position;
       STP3D::Vector4D init_position;
       STP3D::Vector4D direction;
       STP3D::Vector4D init_direction;
       float spotCutoff;
       ShadingModel shadingModel;
       LightType lightType;
    }LightParameters;

    //All lights of the scene
    std::vector<LightParameters> lights;

    //Light ON
    bool lightON;

public:
    LightFactory();

    Light* createPonctualLight(STP3D::Vector3D ambient, STP3D::Vector3D diffuse,
                    STP3D::Vector3D specular, STP3D::Vector3D position,
                             ShadingModel shadingModel=Phong);

    Light* createDirectionalLight(STP3D::Vector3D ambient, STP3D::Vector3D diffuse,
                                STP3D::Vector3D specular, STP3D::Vector3D direction,
                                         ShadingModel shadingModel=Phong);

    Light* createSpotLight(STP3D::Vector3D ambient, STP3D::Vector3D diffuse,
                         STP3D::Vector3D specular, STP3D::Vector3D direction,
                         STP3D::Vector3D position, float cutOff,
                                  ShadingModel shadingModel=Phong);
    /**
     * @brief sendLight send all lights to the current shader
     */
    void sendLight(uint shaderId);

   /*
    *  Set materials component of an object
    */
    void setMaterials(uint shaderId, STP3D::Vector4D color, float ambi, float diff, float spec, float shine);

    /*
     * Enable shading on each mesh
     */
    void enableLigthing();

    /*
     * Disable shading allow direct rendering all mesh only have one color
     */
    void disableLighting();

public:
    /**
     * @brief The Light class
     */
    class Light {
    public :
        void move(Transfo& t);

        /**
         * @brief Light constructor
         * @param lp pointer to LightFactory::LightParameters in LightFactory
         */
        Light(LightParameters * lp) : m_lp(lp) {}

    private:
        LightParameters * m_lp;
    };
};

#endif // LIGHTS_HPP
