// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image.h"

GLuint programIDsquare;
GLuint programIDcubeEx2;
GLuint programIDcubeEx3;
GLuint programIDskybox;
GLuint programIDcubeEx4;

float scale;
Vec3 translate;

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};


unsigned int loadCubemap(std::vector<std::string> facesPath) {
	unsigned int textureID;
	glGenTextures(1 , &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP , textureID);
	int width , height , nrChannels;

	for (unsigned int i = 0; i < facesPath.size(); i++) {
		unsigned char * data = stbi_load(facesPath[i].c_str() , &width , &height , &nrChannels , 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} 
		else {
			std::cout << " Cubemap tex failed : " << facesPath[i] << std::endl;
		}
		stbi_image_free (data) ;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}  


glm::mat4 modifModelMatrix;
static Camera camera;
glm::vec3 camera_position   = glm::vec3(0.0f, 3.0f, 6.0f);
glm::vec3 camera_target = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

struct Mesh {

GLuint squareVertexBuffer, squareNormalBuffer, squareTextureBuffer;
unsigned int squareTexture;

GLuint cubeVertexBuffer, cubeNormalBuffer, cubeTextureBuffer, cubeTangenteBuffer;
GLuint cubeAlbedoTextureBuffer, cubeNormalTextureBuffer, cubeMetallicTextureBuffer, cubeRoughnessTextureBuffer, cubeAOTextureBuffer;
unsigned int cubeTexture, cubeAlbedoTexture, cubeNormalTexture, cubeMetallicTexture, cubeRoughnessTexture, cubeAOTexture;

GLuint skyboxVertexBuffer;
unsigned int cubeMapTexture;
std::vector<std::string> facesPath;


    void initObjectsBuffers() {

        //Ex1
        GLfloat squareVertex[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,

            -1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f
        };

        GLfloat squareNormal[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };

        GLfloat squareTextureData[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,

            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f
        };
        

        //Ex2 & 3
        GLfloat cubeVertex[] = {
            
            //Derriere
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,

            //Devant
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            //Gauche
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,

            //Droite
            0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,

            //Bas
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            //Haut
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f

        };

        //Ex2 & 3
        GLfloat cubeNormals[] = {
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f, -1.0f,

            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,
            0.0f,  0.0f,  1.0f,

            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,

            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,

            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f, -1.0f,  0.0f,

            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f

        };


        GLfloat cubeTextureData[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f,

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f,

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f,

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f,

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f,

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            0.f, 0.f,
            1.f, 1.f,
            1.f, 0.f
        };


        GLfloat cubeTangente[] = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f

        };

        //Ex3
        GLfloat skyboxVertex[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
        };

        //Ex1
        /*glGenBuffers(1, &squareVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertex), &squareVertex, GL_STATIC_DRAW);

        glGenBuffers(1, &squareNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, squareNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareNormal), &squareNormal, GL_STATIC_DRAW);

        glGenBuffers(1, &squareTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, squareTextureBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareTextureData), &squareTextureData, GL_STATIC_DRAW);

        glGenTextures (1 , &squareTexture);
        glBindTexture (GL_TEXTURE_2D , squareTexture);

        int width , height , nbChannels;
        unsigned char * data = stbi_load ( "data/marble.png" , &width , &height , &nbChannels , 0);

        if (data) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width ,height ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data);*/

        //Ex2
        /*glGenBuffers(1, &cubeVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);

        glGenBuffers(1, &cubeNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), &cubeNormals, GL_STATIC_DRAW);

        glGenBuffers(1, &cubeTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeTextureBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureData), &cubeTextureData, GL_STATIC_DRAW);

        glGenTextures (1 , &cubeTexture);
        glBindTexture (GL_TEXTURE_2D , cubeTexture);

        int width , height , nbChannels;
        unsigned char * data = stbi_load ( "data/bump.png" , &width , &height , &nbChannels , 0);

        if (data) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width ,height ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data);

        glGenBuffers(1, &cubeTangenteBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeTangenteBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTangente), &cubeTangente, GL_STATIC_DRAW);*/

        //Ex3
        /*glGenBuffers(1, &cubeVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);

        glGenBuffers(1, &cubeNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), &cubeNormals, GL_STATIC_DRAW);
        
        glGenBuffers(1, &skyboxVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertex), &skyboxVertex, GL_STATIC_DRAW);

        facesPath = {
            "data/right.jpg",
            "data/left.jpg",
            "data/top.jpg",
            "data/bottom.jpg",
            "data/front.jpg",
            "data/back.jpg"
        };

        cubeMapTexture = loadCubemap(facesPath);*/


        //Ex4
        glGenBuffers(1, &cubeVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);

        glGenBuffers(1, &cubeNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), &cubeNormals, GL_STATIC_DRAW);

        glGenBuffers(1, &cubeTextureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, cubeTextureBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureData), &cubeTextureData, GL_STATIC_DRAW);


        glGenTextures (1 , &cubeAlbedoTexture);
        glBindTexture (GL_TEXTURE_2D , cubeAlbedoTexture);

        int width1 , height1 , nbChannels1;
        unsigned char * data1 = stbi_load ( "data/albedoMap.jpg" , &width1 , &height1 , &nbChannels1 , 0);

        if (data1) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width1 ,height1 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data1 );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data1);


        glGenTextures (1 , &cubeNormalTexture);
        glBindTexture (GL_TEXTURE_2D , cubeNormalTexture);

        int width2 , height2 , nbChannels2;
        unsigned char * data2 = stbi_load ( "data/normalMap.jpg" , &width2 , &height2 , &nbChannels2 , 0);

        if (data2) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width2 ,height2 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data2 );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data2);


        glGenTextures (1 , &cubeMetallicTexture);
        glBindTexture (GL_TEXTURE_2D , cubeMetallicTexture);

        int width3 , height3 , nbChannels3;
        unsigned char * data3 = stbi_load ( "data/metallicMap.jpg" , &width3 , &height3 , &nbChannels3 , 0);

        if (data3) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width3 ,height3 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data3 );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data3);


        glGenTextures (1 , &cubeRoughnessTexture);
        glBindTexture (GL_TEXTURE_2D , cubeRoughnessTexture);

        int width4 , height4 , nbChannels4;
        unsigned char * data4 = stbi_load ( "data/roughnessMap.jpg" , &width4 , &height4 , &nbChannels4 , 0);

        if (data4) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width4 ,height4 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data4 );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data4);


        glGenTextures (1 , &cubeAOTexture);
        glBindTexture (GL_TEXTURE_2D , cubeAOTexture);

        int width5 , height5 , nbChannels5;
        unsigned char * data5 = stbi_load ( "data/aoMap.jpg" , &width5, &height5 , &nbChannels5 , 0);

        if (data5) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width5 ,height5 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data5 );
            glGenerateMipmap (GL_TEXTURE_2D );

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }

        stbi_image_free(data5);

    }

    void clearObjectsBuffers(){
        glDeleteBuffers(1, &squareVertexBuffer);
        glDeleteBuffers(1, &squareNormalBuffer);
        glDeleteBuffers(1, &squareTextureBuffer);
        glDeleteBuffers(1, &cubeVertexBuffer);
        glDeleteBuffers(1, &cubeNormalBuffer);
        glDeleteBuffers(1, &cubeTextureBuffer);
        glDeleteBuffers(1, &cubeTangenteBuffer);
        glDeleteBuffers(1, &skyboxVertexBuffer);

    }

    void drawObjectsBuffer (){

        //Ex 1
        /*glUseProgram(programIDsquare);

        glBindBuffer(GL_ARRAY_BUFFER, squareVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, squareNormalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, squareTextureBuffer);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

    
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, squareTexture);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(programIDsquare , "colorTexture"), GL_TEXTURE0);

        glDrawArrays(GL_TRIANGLES, 0, 2*3);*/


        //Ex2
        /*glUseProgram(programIDcubeEx2);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, cubeTextureBuffer);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, cubeTangenteBuffer);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
    
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(programIDsquare , "normalTexture"), GL_TEXTURE0);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);*/


        //Ex3
        /*glUseProgram(programIDcubeEx3);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);

        glUseProgram(programIDskybox);
        
        glDepthFunc(GL_LEQUAL);

        glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        
        glDepthFunc(GL_LESS);*/


        //Ex4
        glUseProgram(programIDcubeEx4);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, cubeTextureBuffer);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeAlbedoTexture);
        glUniform1i(glGetUniformLocation(programIDcubeEx4 , "albedoMap"), 0);

            glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubeNormalTexture);
        glUniform1i(glGetUniformLocation(programIDcubeEx4 , "normalMap"), 1);

            glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cubeMetallicTexture);
        glUniform1i(glGetUniformLocation(programIDcubeEx4 , "metallicMap"), 2);
      
            glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cubeRoughnessTexture);
        glUniform1i(glGetUniformLocation(programIDcubeEx4 , "roughnessMap"), 3);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, cubeAOTexture);
        glUniform1i(glGetUniformLocation(programIDcubeEx4 , "aoMap"), 3);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);

    }
};

Mesh cube;

bool display_normals;
bool display_loaded_mesh;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > & i_vertices ,
              std::vector< Vec3 > & i_normals ,
              std::vector< Triangle > & i_triangles,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}


// ------------------------------------

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    display_normals = false;
    display_loaded_mesh = true;

    scale = .7;

    translate = Vec3(0.,0.,0.);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    modifModelMatrix = glm::mat4(1.f);

}




// ------------------------------------
// rendering.
// ------------------------------------


void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void draw () {
    glClear( GL_COLOR_BUFFER_BIT );

    glm::vec3 position = glm::vec3(2, 0, 0);
	glm::vec3 forward = glm::vec3(0) - position;
	forward = glm::normalize(forward);
	glm::vec3 up = glm::vec3(0, 2, 0);

    float fovAngle = 65.0;
	float aspectRatio = 1.0;
	float nearPlane = 0.001;
	float farPlane = 10000.0;

    //Ex1
    /*glUseProgram(programIDsquare);

    glm::mat4 modelMatrixSquare, viewMatrixSquare, projectionMatrixSquare;

    float X; float Y; float Z;
    camera.getPos(X, Y, Z);
    glm::vec3 camPos = {X, Y, Z};

    viewMatrixSquare = glm::lookAt(camPos, camera_target, camera_up);
    projectionMatrixSquare = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    modelMatrixSquare = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(programIDsquare, "view"), 1 , GL_FALSE, &viewMatrixSquare[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDsquare, "projection"), 1 , GL_FALSE, &projectionMatrixSquare[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDsquare, "model"), 1, false, &modelMatrixSquare[0][0]);*/


    //Ex2
    /*glUseProgram(programIDcubeEx2);

    glm::mat4 modelMatrixCubeEx2, viewMatrixCubeEx2, projectionMatrixCubeEx2;

    float X; float Y; float Z;
    camera.getPos(X, Y, Z);
    glm::vec3 camPos = {X, Y, Z};

    viewMatrixCubeEx2 = glm::lookAt(camPos, camera_target, camera_up);
    projectionMatrixCubeEx2 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    modelMatrixCubeEx2 = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "view"), 1 , GL_FALSE, &viewMatrixCubeEx2[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx2[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "model"), 1, false, &modifModelMatrix[0][0]);

    glm::vec3 objectColor = {1.0, 0.5, 1.0};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "color"), 1, glm::value_ptr(objectColor));


    glm::vec3 vec3LightColor = {1.0, 1.0, 1.0};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "lightColor"), 1, glm::value_ptr(vec3LightColor));


    glm::vec3 vec3LightPos = {-2.0, -1.0, 7.0};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "lightPos"), 1, glm::value_ptr(vec3LightPos));


    glm::vec3 vec3Ambient = {0.21, 0.12, 0.05};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "ambientMaterial"), 1, glm::value_ptr(vec3Ambient));


    glm::vec3 vec3Diffuse = {0.71, 0.42, 0.18};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "diffuseMaterial"), 1, glm::value_ptr(vec3Diffuse));


    glm::vec3 vec3Specular = {0.39, 0.27, 0.16};

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "specularMaterial"), 1, glm::value_ptr(vec3Specular));
    

    float shininess = 0.6;

    glUniform1f(glGetUniformLocation(programIDcubeEx2, "shininessMaterial"), shininess);

    glUniform3fv(glGetUniformLocation(programIDcubeEx2, "cameraPos"), 1, glm::value_ptr(camPos));*/

    //Ex3
    /*glUseProgram(programIDcubeEx3);

    glm::mat4 modelMatrixCube, viewMatrixCube, projectionMatrixCube;
    glm::vec3 pos = glm::vec3(1.75, 0.25, 1.5);

    float X; float Y; float Z;
    camera.getPos(X, Y, Z);
    glm::vec3 camPos = {X, Y, Z};

    viewMatrixCube = glm::lookAt(camPos, glm::vec3(0, 0, 0), camera_up);
    projectionMatrixCube = glm::perspective(glm::radians(45.f), 4.0f/3.0f, 0.1f, 100.0f);
    modelMatrixCube = glm::mat4(1.0f);
    modelMatrixCube = glm::scale(modelMatrixCube, glm::vec3(0.5, 0.5, 0.5));
    modelMatrixCube = glm::translate(modelMatrixCube, glm::vec3(0, -1, 1));

    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx3, "view"), 1 , GL_FALSE, &viewMatrixCube[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx3, "projection"), 1 , GL_FALSE, &projectionMatrixCube[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx3, "model"), 1, false, &modelMatrixCube[0][0]);

    glUniform3fv(glGetUniformLocation(programIDcubeEx3, "cameraPos"), 1, glm::value_ptr(camPos));


    glUseProgram(programIDskybox);

    glDepthFunc(GL_LEQUAL);

    glm::mat4 modelMatrixSkybox, viewMatrixSkybox, projectionMatrixSkybox;

    viewMatrixSkybox = glm::lookAt(position, glm::vec3(0, 0, 0), up);
    projectionMatrixSkybox = glm::perspective(glm::radians(45.f), 4.0f/3.0f, 0.1f, 100.0f);
    modelMatrixSkybox = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "view"), 1 , GL_FALSE, &viewMatrixSkybox[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "projection"), 1 , GL_FALSE, &projectionMatrixSkybox[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1, false, &modelMatrixSkybox[0][0]);*/


    //Ex4
    glUseProgram(programIDcubeEx4);

    glm::mat4 modelMatrixCubeEx4, viewMatrixCubeEx4, projectionMatrixCubeEx4;

    float X; float Y; float Z;
    camera.getPos(X, Y, Z);
    glm::vec3 camPos = {X, Y, Z};

    viewMatrixCubeEx4 = glm::lookAt(camPos, camera_target, camera_up);
    projectionMatrixCubeEx4 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    modelMatrixCubeEx4 = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "view"), 1 , GL_FALSE, &viewMatrixCubeEx4[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx4[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "model"), 1, false, &modelMatrixCubeEx4[0][0]);

    glUniform3fv(glGetUniformLocation(programIDcubeEx4, "cameraPos"), 1, glm::value_ptr(camPos));


    glm::vec3 lightPos = {1.5f, 0.f, 0.0f};

    glm::vec3 lightColor = {15.0, 15.0, 15.0};

    
    glUniform3fv(glGetUniformLocation(programIDcubeEx4, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(programIDcubeEx4, "lightColor"), 1, glm::value_ptr(lightColor));


    cube.drawObjectsBuffer();
}


void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    //float cameraSpeed = 2.5 * deltaTime;

    float posX = 0.0;
    float posY = 0.0;
    float posZ = 0.0;

    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;

    /*case 's':
        camera_position -= cameraSpeed * camera_target;
        break;

    case 'w':
        camera_position += cameraSpeed * camera_target;
        break;
    
    case 'z':
        camera_position -= cameraSpeed * camera_target_lateral;
        break;

    case 'p':
        camera_position += cameraSpeed * camera_target_lateral;
        break;*/


    /*case 'a':
        posX += 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(posX, 0.f, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'b':
        posX -= 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(posX, 0.f, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;
        

    case 'c':
        posY += 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(0.f, posY, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'd':
        posY -= 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(0.f, posY, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'e':
        posZ += 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(0.f, 0.f, posZ));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'g':
        posZ -= 0.5;
        modifModelMatrix = glm::translate(modifModelMatrix, glm::vec3(0.f, 0.f, posZ));
        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;*/

    case 'h':
        modifModelMatrix = glm::rotate(modifModelMatrix, 1.f, glm::vec3(0.5, 0, 0));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'i':
        modifModelMatrix = glm::rotate(modifModelMatrix, -1.f, glm::vec3(0.5, 0, 0));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'j':
        modifModelMatrix = glm::rotate(modifModelMatrix, 1.f, glm::vec3(0, 0.5, 0));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

     case 'k':
        modifModelMatrix = glm::rotate(modifModelMatrix, -1.f, glm::vec3(0, 0.5, 0));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'l':
        modifModelMatrix = glm::rotate(modifModelMatrix, 1.f, glm::vec3(0, 0, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case 'm':
        modifModelMatrix = glm::rotate(modifModelMatrix, -1.f, glm::vec3(0, 0, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;


    case '-': //Press + key to increase scale
        modifModelMatrix = glm::scale(modifModelMatrix, glm::vec3(0.5, 0.5, 0.5));
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx3, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    case '+': //Press - key to decrease scale
        modifModelMatrix = glm::scale(modifModelMatrix, glm::vec3(1.5, 1.5, 1.5));
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx3, "model"), 1 , GL_FALSE, &modifModelMatrix[0][0]);
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}



int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI719I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc(motion);
	glutMouseFunc(mouse);
    key ('?', 0, 0);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //Ex1
    //programIDsquare = load_shaders("shader/ex1_square_vertex_shader.glsl", "shader/ex1_square_fragment_shader.glsl");

    //Ex2
    //programIDcubeEx2 = load_shaders("shader/ex2_cube_vertex_shader.glsl", "shader/ex2_cube_fragment_shader.glsl");

    //Ex3
    /*programIDcubeEx3 = load_shaders("shader/ex3_cube_vertex_shader.glsl", "shader/ex3_cube_fragment_shader.glsl");
    programIDskybox = load_shaders("shader/ex3_skybox_vertex_shader.glsl", "shader/ex3_skybox_fragment_shader.glsl");*/

    //Ex4
    programIDcubeEx4 = load_shaders("shader/ex4_vertex_shader.glsl", "shader/ex4_fragment_shader.glsl");

    cube.initObjectsBuffers();

    glutMainLoop ();

    cube.clearObjectsBuffers();

    glDeleteProgram(programIDsquare);
    glDeleteProgram(programIDcubeEx2);
    glDeleteProgram(programIDcubeEx3);
    glDeleteProgram(programIDskybox);
    glDeleteProgram(programIDcubeEx4);

    return EXIT_SUCCESS;
}

