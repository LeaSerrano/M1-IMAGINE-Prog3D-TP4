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
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint programID;

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


struct TriangleVArray {

    GLuint vertexbuffer, colorbuffer, texbuffer;
    unsigned int texture;

    void initBuffers(){
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
        };

        static const GLfloat g_color_buffer_data[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        static const GLfloat g_texture_buffer_data[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
        };

        // Creer un premier buffer contenant les positions
        // a mettre dans le layout 0
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data, GL_STATIC_DRAW);

        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);

        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), &g_color_buffer_data, GL_STATIC_DRAW);


        glGenBuffers(1, &texbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_buffer_data), &g_texture_buffer_data, GL_STATIC_DRAW);

        glGenTextures (1 , &texture );
        glBindTexture (GL_TEXTURE_2D , texture);
        // load and generate the texture
        int width , height , nrChannels;
        unsigned char * data = stbi_load ( "marble.png" , &width , &height , &nrChannels , 0);
        if (data) {
            glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,width ,height ,0 ,GL_RGB ,GL_UNSIGNED_BYTE ,data );
            glGenerateMipmap (GL_TEXTURE_2D );
            //std::cout << "Success" << std::endl;

        } else {
            std::cout << " Failed to load texture " << std::endl ;
        }
        stbi_image_free(data);

    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers

        glDeleteBuffers(1, &vertexbuffer); 
        glDeleteBuffers(1, &colorbuffer);
        glDeleteBuffers(1, &texbuffer);
        glDeleteTextures (1 , & texture ) ;

    }

    void draw (){
        // 1rst attribute buffer : vertices
        //A faire
        //Utiliser glVertexAttribPointer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);//permet de dire quel buffer on va manipuler
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);//stride = combien il y a de bits qui séparent 2 positions (en gros la taille d'une composante)
        glEnableVertexAttribArray(0);

        //Ajouter un attribut dans un color buffer à envoyé au GPU
        //Utiliser glVertexAttribPointer
        // 2nd attribute buffer : normals
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, texbuffer);//permet de dire quel buffer on va manipuler
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);//stride = combien il y a de bits qui séparent 2 positions (en gros la taille d'une composante)
        glEnableVertexAttribArray(2);


        // activate usage of 2 D textures
        glEnable ( GL_TEXTURE_2D ) ;
        // activate texture ( limited to 32 or less depending on machine )
        int indexActiveTexture = 0; // 0 to 31
        // bind texture as Texture 0
        glBindTexture ( GL_TEXTURE_2D , texture ) ;
        glActiveTexture ( GL_TEXTURE0 + indexActiveTexture) ;
        // set used active texture ( Modern OpenGL )
        glUniform1i ( glGetUniformLocation( programID , " tex " ) , indexActiveTexture) ;


        // Draw the triangle !
        // Utiliser glDrawArrays
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //Pensez à desactive les AttributArray

        glUniform1f(glGetUniformLocation(programID, "scale"), scale);
    }
};


struct Mesh {
    std::vector< Vec3 > vertices;
    std::vector< Vec3 > normals;
    std::vector< Triangle > triangles;

    GLuint vertexbuffer, colorbuffer, elementbuffer;

    void initTriangleMesh(){
        std::vector<Vec3> g_vertex_buffer_data {
            Vec3(-1.0f, -1.0f, 0.0f),
                    Vec3(1.0f, -1.0f, 0.0f),
                    Vec3(1.0f,  1.0f, 0.0f),
        };

        std::vector<Vec3> g_color_buffer_data {
            Vec3(1.0f, 0.0f, 0.0f),
                    Vec3(0.0f, 1.0f, 0.0f),
                    Vec3(0.0f, 0.0f, 1.0f),
        };

        vertices = g_vertex_buffer_data;
        normals = g_color_buffer_data;

        triangles.push_back( Triangle(0,1,2) );
    }

    void initBuffers(){

        // Creer un premier buffer contenant les positions
        // a mettre dans le layout 0
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);

        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);
        // Piste : utiliser sizeof(Vec3)

        //Remplir indices avec la liste des indices des triangles concatenes
        std::vector<unsigned int> indices;

        // Creer un element buffer contenant les indices des sommets
        // Utiliser
        // glGenBuffers(...);
        // glBindBuffer(...);
        // glBufferData(...);


    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
    }

    void draw (){
        // 1rst attribute buffer : vertices
        //A faire
        //Utiliser glVertexAttribPointer

        //Ajouter un attribut dans un color buffer à envoyé au GPU
        //Utiliser glVertexAttribPointer
        // 2nd attribute buffer : normals


        // Draw the triangles !
        // Utiliser l'index buffer
        // glBindBuffer
        // glDrawElements

        //Pensez à desactive les AttributArray

    }
};


TriangleVArray first_triangle;
Mesh triangle_mesh;

Mesh mesh;



bool display_normals;
bool display_loaded_mesh;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
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

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
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


void drawNormals( Mesh const & i_mesh ) {

    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_mesh.normals.size() ; ++pIt) {
        Vec3 to = i_mesh.vertices[pIt] + 0.02*i_mesh.normals[pIt];
        drawVector(i_mesh.vertices[pIt], to);
    }
}

void draw () {

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    // Definition des parametre pour le rendu : uniforms etc...
    // ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
    // Utiliser glGetUniformLocation pour récuperer l'identifiant GLuint
    // Ensuite glUniform1f( id_recuperer , valeur );
    // ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

    // Ajouter une translation en envoyant un vec3

    //Dessin du premier triangle
    first_triangle.draw();

    //Definir une translation entre les 2 si vous le souhaitez

    //Ajouter le dessin du triangle en tant que liste indexée : maillages
    triangle_mesh.draw();

    //Ajouter si vous le souhaitez le maillage mesh
    //mesh.draw();

    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }
    glEnable(GL_LIGHTING);
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


    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;


    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;


    case '+': //Press + key to increase scale
        //Completer augmenter la valeur de la variable scale e.g. +0.005
        scale += 0.005;
        glUniform1f(glGetUniformLocation(programID, "scale"), scale);
        break;

    case '-': //Press - key to decrease scale
        //Completer
        scale -= 0.005;
        glUniform1f(glGetUniformLocation(programID, "scale"), scale);
        break;

    case 'd': //Press d key to translate on x positive
        //Completer : mettre à jour le x du Vec3 translate
        translate += Vec3(0.5, 0, 0);
        glUniform3f(glGetUniformLocation(programID, "translation"), translate[0], translate[1], translate[2]);
        break;

    case 'q': //Press q key to translate on x negative
        //Completer : mettre à jour le y du Vec3 translate
        translate += Vec3(-0.5, 0, 0);
        glUniform3f(glGetUniformLocation(programID, "translation"), translate[0], translate[1], translate[2]);
        break;

    case 'z': //Press z key to translate on y positive
        //Completer : mettre à jour le y du Vec3 translate
        translate += Vec3(0, 0.5, 0);
        glUniform3f(glGetUniformLocation(programID, "translation"), translate[0], translate[1], translate[2]);
        break;

    case 's': //Press s key to translate on y negative
        //Completer : mettre à jour le y du Vec3 translate
        translate += Vec3(0, -0.5, 0);
        glUniform3f(glGetUniformLocation(programID, "translation"), translate[0], translate[1], translate[2]);
        break;

    case '1': //Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
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
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Look into data to find other meshes
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles);

    //Construction d'un maillage contenant un triangle
    triangle_mesh.initTriangleMesh();

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    programID = load_shaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    //Initialisation des buffers : fonction à completer
    first_triangle.initBuffers();
    triangle_mesh.initBuffers();
    mesh.initBuffers();

    glutMainLoop ();

    // Cleanup VBO and shader
    mesh.clearBuffers();
    triangle_mesh.clearBuffers();
    first_triangle.clearBuffers();

    //Liberation de la memoire
    glDeleteProgram(programID);

    return EXIT_SUCCESS;
}

