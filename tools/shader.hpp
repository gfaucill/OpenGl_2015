#ifndef SHADER_HPP
#define SHADER_HPP

/*
 *  Provide methods to send matrix in the shader
 */
class ShaderTools {
public:
    static void sendViewMatrix(double mat[16]) {
        ShaderTools::sendMatrix(mat, (char*)"viewMatrix");
    }

    static void sendViewMatrix(float mat[16]) {
        sendMatrix(mat, (char*)"viewMatrix");
    }

    static void sendModelMatrix(float mat[16]) {
        sendMatrix(mat, (char*)"modelMatrix");
    }

    static void sendModelMatrix(double mat[16]) {
        sendMatrix(mat, (char*)"modelMatrix");
    }

    static void sendNormalMatrix(float mat[16]) {
        sendMatrix(mat, (char*)"normalMatrix");
    }

    static void sendNormalMatrix(double mat[16]) {
        sendMatrix(mat, (char*)"normalMatrix");
    }

    static void sendProjectionMatrix(float mat[16]) {
        sendMatrix(mat, (char*)"projectionMatrix");
    }

    static void sendProjectionMatrix(double mat[16]) {
        sendMatrix(mat, (char*)"projectionMatrix");
    }

private:
    inline static void sendMatrix(float mat[16], char *name) {
        int id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);
        glUniformMatrix4fv(glGetUniformLocation(id,name),1,GL_FALSE,mat);
    }

    inline static void sendMatrix(double mat[16], char *name) {
        float m[] = { (float)mat[0], (float)mat[1], (float)mat[2], (float)mat[3],
                      (float)mat[4], (float)mat[5], (float)mat[6], (float)mat[7],
                      (float)mat[8], (float)mat[9], (float)mat[10], (float)mat[11],
                      (float)mat[12], (float)mat[13], (float)mat[14], (float)mat[15] };

        sendMatrix(m,name);
    }
};

#endif // SHADER_HPP
