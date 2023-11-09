#pragma once
#include <vector>
#include <glm/glm.hpp>

class cells {
	unsigned int vbo[2];
	unsigned int vao[2];
	unsigned int ebo[2];
	unsigned int texture[2];
	unsigned int programID;
	std::vector<unsigned int> indices[2];
	std::vector<float> vertexdataholder[2];
	float cTextureID;


	void update_indices(int id);
	int exists(const std::vector<glm::vec3>& ,int);
	void initbuffers();
	void removevertex(int,int);

public:
	
	cells();
	bool add(std::vector<glm::vec3>, int id);
	bool update();
	void draw();
	unsigned int get_programID();
	void switchTexture();
};