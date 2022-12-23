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

GLuint programIDcubeEx1;
GLuint programIDcubeEx2;
GLuint programIDcubeEx3;
GLuint programIDskybox;
GLuint programIDcubeEx4;
GLuint programIDcubeEx5;

float scale;
Vec3 translate;

char * exerciceSelectionne;

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

struct Mesh {

GLuint cubeVertexBuffer, cubeNormalBuffer, cubeTextureBuffer, cubeTangenteBuffer;
GLuint cubeAlbedoTextureBuffer, cubeNormalTextureBuffer, cubeMetallicTextureBuffer, cubeRoughnessTextureBuffer, cubeAOTextureBuffer;
GLuint cubeLightVertexBuffer;
unsigned int cubeTexture, cubeAlbedoTexture, cubeNormalTexture, cubeMetallicTexture, cubeRoughnessTexture, cubeAOTexture;

GLuint skyboxVertexBuffer;
unsigned int cubeMapTexture;
std::vector<std::string> facesPath;


    void initObjectsBuffers() {

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

            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,

            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,

            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,
            0.0f,  1.0f,  0.0f,

            0.0f,  -1.0f,  0.0f,
            0.0f,  -1.0f,  0.0f,
            0.0f,  -1.0f,  0.0f,
            0.0f,  -1.0f,  0.0f,
            0.0f,  -1.0f,  0.0f,
            0.0f,  -1.0f,  0.0f

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
        if (exerciceSelectionne == "Ex1") {
            glGenBuffers(1, &cubeVertexBuffer);
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
            unsigned char * data = stbi_load ( "data/brick.jpg" , &width , &height , &nbChannels , 0);

            if (data) {
                glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width ,height ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data );
                glGenerateMipmap (GL_TEXTURE_2D );

            } else {
                std::cout << " Failed to load texture " << std::endl ;
            }

            stbi_image_free(data);
        }

        //Ex2
        if (exerciceSelectionne == "Ex2") {
            glGenBuffers(1, &cubeVertexBuffer);
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
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTangente), &cubeTangente, GL_STATIC_DRAW);
        }

        //Ex3
        if (exerciceSelectionne == "Ex3") {
            glGenBuffers(1, &cubeVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);

            glGenBuffers(1, &cubeNormalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormals), &cubeNormals, GL_STATIC_DRAW);
        
            glGenBuffers(1, &skyboxVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertex), &skyboxVertex, GL_STATIC_DRAW);

            facesPath = {
                "data/Skybox/Sea/right.jpg",
                "data/Skybox/Sea/left.jpg",
                "data/Skybox/Sea/top.jpg",
                "data/Skybox/Sea/bottom.jpg",
                "data/Skybox/Sea/front.jpg",
                "data/Skybox/Sea/back.jpg"
            };

            cubeMapTexture = loadCubemap(facesPath);
        }


        //Ex4
        if (exerciceSelectionne == "Ex4" || exerciceSelectionne == "Ex5") {
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
            unsigned char * data1 = stbi_load ( "data/PBR/Wood/albedo.jpg" , &width1 , &height1 , &nbChannels1 , 0);

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
            unsigned char * data2 = stbi_load ( "data/PBR/Wood/normal.jpg" , &width2 , &height2 , &nbChannels2 , 0);

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
            unsigned char * data3 = stbi_load ( "data/PBR/Wood/metallic.jpg" , &width3 , &height3 , &nbChannels3 , 0);

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
            unsigned char * data4 = stbi_load ( "data/PBR/Wood/roughness.jpg" , &width4 , &height4 , &nbChannels4 , 0);

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
            unsigned char * data5 = stbi_load ( "data/PBR/Wood/ao.jpg" , &width5, &height5 , &nbChannels5 , 0);

            if (data5) {
                glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width5 ,height5 ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data5 );
                glGenerateMipmap (GL_TEXTURE_2D );

            } else {
                std::cout << " Failed to load texture " << std::endl ;
            }

            stbi_image_free(data5);
        }


        //Ex5
        if (exerciceSelectionne == "Ex5") {
            glGenBuffers(1, &cubeLightVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, cubeLightVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);
        }

    }

    void clearObjectsBuffers(){
        glDeleteBuffers(1, &cubeVertexBuffer);
        glDeleteBuffers(1, &cubeNormalBuffer);
        glDeleteBuffers(1, &cubeTextureBuffer);
        glDeleteBuffers(1, &cubeTangenteBuffer);
        glDeleteBuffers(1, &skyboxVertexBuffer);
        glDeleteBuffers(1, &cubeLightVertexBuffer);

    }

    void drawObjectsBuffer (){

        //Ex 1
        if (exerciceSelectionne == "Ex1") {
            glUseProgram(programIDcubeEx1);

            glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, cubeNormalBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, cubeTextureBuffer);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
            glEnableVertexAttribArray(2);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, cubeTexture);
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(programIDcubeEx1 , "colorTexture"), GL_TEXTURE0);

            glDrawArrays(GL_TRIANGLES, 0, 12*3);
        }


        //Ex2
        if (exerciceSelectionne == "Ex2") {
            glUseProgram(programIDcubeEx2);

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
            glUniform1i(glGetUniformLocation(programIDcubeEx2 , "normalTexture"), GL_TEXTURE0);

            glDrawArrays(GL_TRIANGLES, 0, 12*3);
        }


        //Ex3
        if (exerciceSelectionne == "Ex3") {
            glUseProgram(programIDcubeEx3);

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

            glDepthFunc(GL_LESS);
        }


        //Ex4
        if (exerciceSelectionne == "Ex4" || exerciceSelectionne == "Ex5") {
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


        //Ex5
        if (exerciceSelectionne == "Ex5") {
            glUseProgram(programIDcubeEx5);

            glBindBuffer(GL_ARRAY_BUFFER, cubeLightVertexBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, 12*3);
        }

    }
};

Mesh cube;

bool display_normals;
bool display_loaded_mesh;

glm::vec3 position = glm::vec3(1.75, 0, 0);
glm::vec3 up = glm::vec3(0, 2, 0);

static Camera camera;
glm::vec3 camera_position   = glm::vec3(0.0f, 3.0f, 6.0f);
glm::vec3 camera_target = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

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

    float X; float Y; float Z;
    camera.getPos(X, Y, Z);
    glm::vec3 camPos = {X, Y, Z};

    //Ex1
    if (exerciceSelectionne == "Ex1") {
        glUseProgram(programIDcubeEx1);

        glm::mat4 modelMatrixCubeEx1, viewMatrixCubeEx1, projectionMatrixCubeEx1;

        viewMatrixCubeEx1 = glm::lookAt(camPos, camera_target, camera_up);
        projectionMatrixCubeEx1 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        modelMatrixCubeEx1 = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx1, "view"), 1 , GL_FALSE, &viewMatrixCubeEx1[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx1, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx1[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx1, "model"), 1, false, &modelMatrixCubeEx1[0][0]);
    }


    //Ex2
    if (exerciceSelectionne == "Ex2") {
        glUseProgram(programIDcubeEx2);

        glm::mat4 modelMatrixCubeEx2, viewMatrixCubeEx2, projectionMatrixCubeEx2;

        viewMatrixCubeEx2 = glm::lookAt(camPos, camera_target, camera_up);
        projectionMatrixCubeEx2 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        modelMatrixCubeEx2 = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "view"), 1 , GL_FALSE, &viewMatrixCubeEx2[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx2[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx2, "model"), 1, false, &modelMatrixCubeEx2[0][0]);

        glm::vec3 objectColor = {1.0, 0.5, 1.0};
        glUniform3fv(glGetUniformLocation(programIDcubeEx2, "color"), 1, glm::value_ptr(objectColor));


        glm::vec3 vec3LightColor = {0.5, 0.5, 0.5};
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

        glUniform3fv(glGetUniformLocation(programIDcubeEx2, "cameraPos"), 1, glm::value_ptr(camPos));
    }

    //Ex3
    if (exerciceSelectionne == "Ex3") {
        glUseProgram(programIDcubeEx3);

        glm::mat4 modelMatrixCube, viewMatrixCube, projectionMatrixCube;

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

        glm::mat4 modelMatrixSkybox, viewMatrixSkybox, projectionMatrixSkybox;

        viewMatrixSkybox = glm::lookAt(position, glm::vec3(0, 0, 0), camera_up);

        projectionMatrixSkybox = glm::perspective(glm::radians(45.f), 4.0f/3.0f, 0.1f, 100.0f);
        modelMatrixSkybox = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "view"), 1 , GL_FALSE, &viewMatrixSkybox[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "projection"), 1 , GL_FALSE, &projectionMatrixSkybox[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDskybox, "model"), 1, false, &modelMatrixSkybox[0][0]);
    }


    //Ex4
    if (exerciceSelectionne == "Ex4" || exerciceSelectionne == "Ex5") {
        glUseProgram(programIDcubeEx4);

        glm::mat4 modelMatrixCubeEx4, viewMatrixCubeEx4, projectionMatrixCubeEx4;

        viewMatrixCubeEx4 = glm::lookAt(camPos, camera_target, camera_up);
        projectionMatrixCubeEx4 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        modelMatrixCubeEx4 = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "view"), 1 , GL_FALSE, &viewMatrixCubeEx4[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx4[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx4, "model"), 1, false, &modelMatrixCubeEx4[0][0]);

        glUniform3fv(glGetUniformLocation(programIDcubeEx4, "cameraPos"), 1, glm::value_ptr(camPos));


        glm::vec3 lightPos = {1.f, 1.f, 0.0f};

        glm::vec3 lightColor = {15.0, 15.0, 15.0};


        glUniform3fv(glGetUniformLocation(programIDcubeEx4, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(programIDcubeEx4, "lightColor"), 1, glm::value_ptr(lightColor));
    }

    //Ex5
    if (exerciceSelectionne == "Ex5") {
        glUseProgram(programIDcubeEx5);
    
        glm::mat4 modelMatrixCubeEx5, viewMatrixCubeEx5, projectionMatrixCubeEx5;
    
        glm::vec3 lightPos = {1.f, 1.f, 0.0f};
    
        viewMatrixCubeEx5 = glm::lookAt(camPos, camera_target, camera_up);
        projectionMatrixCubeEx5 = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        modelMatrixCubeEx5 = glm::mat4(1.0f);
        modelMatrixCubeEx5 = glm::translate(modelMatrixCubeEx5, lightPos);
        modelMatrixCubeEx5 = glm::scale(modelMatrixCubeEx5, glm::vec3(0.2f));
    
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx5, "view"), 1 , GL_FALSE, &viewMatrixCubeEx5[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx5, "projection"), 1 , GL_FALSE, &projectionMatrixCubeEx5[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programIDcubeEx5, "model"), 1, false, &modelMatrixCubeEx5[0][0]);
    }
    

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

    if (std::string(argv[1]) == "Ex1") {
        exerciceSelectionne = "Ex1";
    }
    else if (std::string(argv[1]) == "Ex2") {
        exerciceSelectionne = "Ex2";
    }
    else if (std::string(argv[1]) == "Ex3") {
        exerciceSelectionne = "Ex3";
    }
    else if (std::string(argv[1]) == "Ex4") {
        exerciceSelectionne = "Ex4";
    }
    else if (std::string(argv[1]) == "Ex5") {
        exerciceSelectionne = "Ex5";
    }


    //Ex1
    if (exerciceSelectionne == "Ex1") {
        programIDcubeEx1 = load_shaders("shader/ex1_cube_vertex_shader.glsl", "shader/ex1_cube_fragment_shader.glsl");
    }

    //Ex2
    if (exerciceSelectionne == "Ex2") {
        programIDcubeEx2 = load_shaders("shader/ex2_cube_vertex_shader.glsl", "shader/ex2_cube_fragment_shader.glsl");
    }

    //Ex3
    if (exerciceSelectionne == "Ex3") {
        programIDcubeEx3 = load_shaders("shader/ex3_cube_vertex_shader.glsl", "shader/ex3_cube_fragment_shader.glsl");
        programIDskybox = load_shaders("shader/ex3_skybox_vertex_shader.glsl", "shader/ex3_skybox_fragment_shader.glsl");
    }

    //Ex4
    if (exerciceSelectionne == "Ex4" || exerciceSelectionne == "Ex5") {
        programIDcubeEx4 = load_shaders("shader/ex4_vertex_shader.glsl", "shader/ex4_fragment_shader.glsl");
    }

    //Ex5
    if (exerciceSelectionne == "Ex5") {
        programIDcubeEx5 = load_shaders("shader/ex5_vertex_shader.glsl", "shader/ex5_fragment_shader.glsl");
    }

    cube.initObjectsBuffers();

    glutMainLoop ();

    cube.clearObjectsBuffers();

    glDeleteProgram(programIDcubeEx1);
    glDeleteProgram(programIDcubeEx2);
    glDeleteProgram(programIDcubeEx3);
    glDeleteProgram(programIDskybox);
    glDeleteProgram(programIDcubeEx4);
    glDeleteProgram(programIDcubeEx5);

    return EXIT_SUCCESS;
}

