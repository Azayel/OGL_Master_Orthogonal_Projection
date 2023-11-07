#pragma once
#include <vector>
#include <glm/glm.hpp>

class cells {
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	unsigned int texture;
	unsigned int programID;
	std::vector<unsigned int> indices;
	std::vector<float> vertexdataholder;
	float cTextureID;


	void update_indices();
	bool exists(const std::vector<glm::vec3>&);

public:
	
	cells();
	bool add(std::vector<glm::vec3>);
	bool update();
	void draw();
	unsigned int get_programID();
	void switchTexture();
};