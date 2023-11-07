#include "cells.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include <string>

//Yoinked from @Cherno example BatchRendering Textures minute 7.14
static unsigned int LoadTexture(const std::string& path) {
    
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);
    unsigned int textureID;
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb);
    if (pixels) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(pixels);

    return textureID;
}



cells::cells() {
	//Gen Buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

    //Textures from here
    
    //this->texture = LoadTexture("missingtexture.jpeg");


    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("missingtexture.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);




    //Textures to here

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexdataholder.size() * sizeof(float), vertexdataholder.data(), GL_STATIC_DRAW);

    // float.x float.y float.z float.xt float.yt float.id
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    Shader myShader("cell_shader.vert", "cell_shader.frag");
    this->programID = myShader.getID();
}





bool cells::add(std::vector<glm::vec3> v_add) {


    //Setup should look like the follogin
    //One Vertex has following information in the vertexdataholder
    //   float,float,float <--- Vertex position
    // + float,float       <--- Texture coordinates
    // + float             <--- texture id
    //----------------------------------
    // = 6 * float per vertex



    if (v_add.size() != 4) {
        std::cout << "std::vector.size() is not 4.\n";
        return false;
    }
    /*
    if (this->exists(v_add)) {
        return false;
    }

    TODO Exists modify for new vertex position alligment
    */

    std::vector<glm::vec2> texCoords = {
        glm::vec2(0.0f, 0.0f),  //First Vertex Left bottom
        glm::vec2(1.0f, 0.0f),  //lower right
        glm::vec2(1.0f, 1.0f),  //top right
        glm::vec2(0.0f, 1.0f)   //top left
    };
    /*
    this->vertices.insert(this->vertices.end(), { v_add[0],v_add[1] ,v_add[2] ,v_add[3] });
    this->textures.insert(this->textures.end(), texCoords.begin(), texCoords.end());
    */

    //Dont know better way right now. O(n) isnt it ? yikes

    for (int i = 0; i < 4; i++) {
        this->vertexdataholder.push_back(v_add[i].x);
        this->vertexdataholder.push_back(v_add[i].y);
        this->vertexdataholder.push_back(v_add[i].z);
        this->vertexdataholder.push_back(texCoords[i].x);
        this->vertexdataholder.push_back(texCoords[i].y);
        this->vertexdataholder.push_back(0.0f);
    }


    //and the last step add the good id of the texture 0.0f for the holy missing texture of gmod :=)



    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexdataholder.size() * sizeof(float), vertexdataholder.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    this->update_indices();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    return true;
}


void cells::update_indices() {
    

    
    if (indices.size() == 0) {
        // If indices are empty, initialize them for the first quad.
        indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });
    }
    else {

        //  float.x, float.y, float.z, float, floa, float, float.x ....
        // Calculate the base index based on the number of vertices.
        int s = indices.size() / 1.5f;
        for (unsigned int i = 0; i < 6; i++) {
            // Update indices based on the new vertices in vertexdataholder.
            //indices.push_back(baseIndex + indices[i]);
            //indices.insert(indices.end(), { 4,5,6,6,7,4 });
            indices.insert(indices.end(), s + indices[i]);
        }
    }

    


    
}

void cells::draw() {
    //glBindTextureUnit(0, texture[0]);
    //glBindTextureUnit(1, texture[1]);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}


/*
bool cells::exists(const std::vector<glm::vec3>& tosearch) {


    if (vertexdataholder.size() % 6 != 0) {
        std::cout << "Impossible to search if exist due to error in stack, can't be divided by 4.\n";
        return false; // Return false to indicate that the search is impossible.
    }


    bool match = false;

    for (size_t i = 0; i < vertexdataholder.size(); i += 6) {
        for (int j = 0; i < 4; j++) {
            if (tosearch[j].x == vertexdataholder[i + j] && tosearch[j].y == vertexdataholder[i + j + 1]) {
                match = true;
            }
            else {
                match = false;
            }
        }
       
    }

    std::cout << "Match not found\n";
    return match;  // If no match is found, return false.
}
*/

unsigned int cells::get_programID(){
    return programID;
}


void cells::switchTexture() {
    if (cTextureID == 1.0f) {
        cTextureID = 0.0f;
    }
    else {
        cTextureID++;
    }
}

