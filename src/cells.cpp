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
    this->initbuffers();

    Shader myShader("../src/cell_shader.vert", "../src/cell_shader.frag");
    this->programID = myShader.getID();
}


void cells::initbuffers() {
    glGenBuffers(2, vbo);
    glGenBuffers(2, ebo);
    glGenVertexArrays(2, vao);

    this->texture[0] = LoadTexture("../textures/warped.jpg");
    this->texture[1] = LoadTexture("../textures/nether.jpg");

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    glBufferData(GL_ARRAY_BUFFER, vertexdataholder[0].size() * sizeof(float), vertexdataholder[0].data(), GL_STATIC_DRAW);

    // float.x float.y float.z float.xt float.yt float.id
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[0].size() * sizeof(unsigned int), indices[0].data(), GL_STATIC_DRAW);

    //////////


    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    glBufferData(GL_ARRAY_BUFFER, vertexdataholder[1].size() * sizeof(float), vertexdataholder[1].data(), GL_STATIC_DRAW);

    // float.x float.y float.z float.xt float.yt float.id
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[1].size() * sizeof(unsigned int), indices[1].data(), GL_STATIC_DRAW);

}



void cells::removevertex(int firstvertexindex, int id) {
    std::cout << "Removing from: " << id << " " << "Size of if buffer " << vertexdataholder[id].size() << " " << "Size of the indices buffer " << indices[id].size() << std::endl;

    this->vertexdataholder[id].erase(vertexdataholder[id].begin() + firstvertexindex, vertexdataholder[id].begin() + firstvertexindex + 24);
    
    int a = firstvertexindex / 4;
    this->indices[id].erase(indices[id].begin() + indices[id].size()-6, indices[id].begin() + indices[id].size());
    std::cout << "Removing from: " << id << " " << "Size of if buffer " << vertexdataholder[id].size() << " " << "Size of the indices buffer " << indices[id].size() << std::endl;

    //Update respected buffers

    glBindVertexArray(vao[id]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[id]);
    glBufferData(GL_ARRAY_BUFFER, vertexdataholder[id].size() * sizeof(float), vertexdataholder[id].data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[id]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[id].size() * sizeof(unsigned int), indices[id].data(), GL_STATIC_DRAW);


}



bool cells::add(std::vector<glm::vec3> v_add, int id) {


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


    int id0 = this -> exists(v_add, 0);
    int id1 = this-> exists(v_add, 1);

    if (id0 != -1) {
        this->removevertex(id0, 0); //warped 
    }
    else if (id1 != -1) {
        this->removevertex(id1, 1); //nether
    }

    

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
        this->vertexdataholder[id].push_back(v_add[i].x);
        this->vertexdataholder[id].push_back(v_add[i].y);
        this->vertexdataholder[id].push_back(v_add[i].z);
        this->vertexdataholder[id].push_back(texCoords[i].x);
        this->vertexdataholder[id].push_back(texCoords[i].y);
        this->vertexdataholder[id].push_back(0.0f);
    }


    //and the last step add the good id of the texture 0.0f for the holy missing texture of gmod :=)
    


    glBindVertexArray(vao[id]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[id]);
    glBufferData(GL_ARRAY_BUFFER, vertexdataholder[id].size() * sizeof(float), vertexdataholder[id].data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[id]);
    this->update_indices(id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[id].size() * sizeof(unsigned int), indices[id].data(), GL_STATIC_DRAW);

    std::cout << "Removing from: " << id << " " << "Size of if buffer " << vertexdataholder[id].size() << " " << "Size of the indices buffer " << indices[id].size() << std::endl;

    return true;
}


void cells::update_indices(int id) {
    

    
    if (indices[id].size() == 0) {
        // If indices are empty, initialize them for the first quad.
        indices[id].insert(indices[id].end(), { 0, 1, 2, 2, 3, 0 });
    }
    else {

        //  float.x, float.y, float.z, float, floa, float, float.x ....
        // Calculate the base index based on the number of vertices.
        int s = indices[id].size() / 1.5f;
        for (unsigned int i = 0; i < 6; i++) {
            // Update indices based on the new vertices in vertexdataholder.
            //indices.push_back(baseIndex + indices[i]);
            //indices.insert(indices.end(), { 4,5,6,6,7,4 });
            indices[id].insert(indices[id].end(), s + indices[id][i]);
        }
    }

    
}

void cells::draw() {
    //glBindTextureUnit(0, texture[0]);
    //glBindTextureUnit(1, texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glDrawElements(GL_TRIANGLES, indices[0].size(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glDrawElements(GL_TRIANGLES, indices[1].size(), GL_UNSIGNED_INT, 0);

}



int cells::exists(const std::vector<glm::vec3>& tosearch, int index) {

    //One Vertex has following information in the vertexdataholder
   //   float,float,float <--- Vertex position
   // + float,float       <--- Texture coordinates
   // + float             <--- texture id
   //----------------------------------
   // = 6 * float per vertex


    if (vertexdataholder[index].size() % 6 != 0) {
        std::cout << "Impossible to search if exist due to error in stack, can't be divided by 4.\n";
        return false; // Return false to indicate that the search is impossible.
    }

    //given vertexholder we need to compare 4 verticies.
    int vertexIndexCounter = 0;
    bool match = false;
    //YUCK DONT JUDGE ME PLEASE
    for (int i = 0; i < vertexdataholder[index].size(); i += 6) {
        int a = i;
        if (vertexdataholder[index][a] == tosearch[vertexIndexCounter].x && vertexdataholder[index][++a] == tosearch[vertexIndexCounter].y) {
            match = true;
        }
        else {
            match = false;
        }
        if(vertexIndexCounter == 3){
            if (match) {
                std::cout << "Match Found!\n";
                return i-6*3;
            }
            else {
                vertexIndexCounter = 0;
                match = false;
            }
        }
        else {
            vertexIndexCounter++;
        }
    }

    std::cout << "Match not found\n";
    return -1;  // If no match is found, return -1.
}




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

