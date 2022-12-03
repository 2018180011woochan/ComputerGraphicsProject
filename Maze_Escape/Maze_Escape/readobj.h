//#pragma once
//#include <string>
//#include <gl/glm/glm.hpp>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <iostream>
//void readobj(const std::string objfilename, std::vector<glm::vec4>& vertex);

#pragma once
#include<string>
#include<gl/glm/glm.hpp>
#include<fstream>
#include<vector>
#include<string>
#include<iostream>
void readTriangleObj(const std::string objfilename, std::vector<glm::vec4>& vertex, std::vector<glm::vec2>& vertexTexture, std::vector<glm::vec4>& vertexNomal);

