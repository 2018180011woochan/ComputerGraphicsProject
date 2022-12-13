#pragma once
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<gl/glm/glm.hpp>
#include<gl/glm/ext.hpp>
#include<gl/glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<math.h>
#include<fstream>
#include<string>
#include "filetobuf.h"

void makeVertexShader();
void makeFragmentShader();
void makeShaderID();
GLuint fragmentShader;
GLuint effectfragmentShader;

GLuint modelvertexShader;
GLuint effectvertexShader;

GLuint shaderID;
GLuint EffectShaderID;


char* vertexSource;
char* vertexSource2;
char* fragmentSource;
char* fragmentSource2;


void makeVertexShader()
{
	vertexSource = filetobuf("Shader/VertexTextureShader.glsl");
	modelvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(modelvertexShader, 1, &vertexSource, NULL);
	glCompileShader(modelvertexShader);

	vertexSource2 = filetobuf("Shader/vertex.glsl");
	effectvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(effectvertexShader, 1, &vertexSource2, NULL);
	glCompileShader(effectvertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(modelvertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(modelvertexShader, 512, NULL, errorLog);
		cerr << "VERTEXSHADER ERROR: " << errorLog << endl;
	}
}

void makeFragmentShader()
{
	fragmentSource = filetobuf("Shader/fragmentBlendShader.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	fragmentSource2 = filetobuf("Shader/fragment.glsl");
	effectfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(effectfragmentShader, 1, &fragmentSource2, NULL);
	glCompileShader(effectfragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "FRAGMENT SHADER ERROR: " << errorLog << endl;
	}
}

void makeShaderID()
{
	makeVertexShader();
	makeFragmentShader();

	shaderID = glCreateProgram();

	glAttachShader(shaderID, modelvertexShader);
	glAttachShader(EffectShaderID, effectvertexShader);
	glAttachShader(shaderID, fragmentShader);
	glAttachShader(EffectShaderID, effectfragmentShader);

	glLinkProgram(shaderID);
	glLinkProgram(EffectShaderID);

	GLint result;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	//glGetProgramiv(EffectShaderID, GL_LINK_STATUS, &result);
	GLchar errorLog[512];
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		cerr << "ShaderID0 Program ERROR: " << errorLog << endl;
	}

	glDeleteShader(modelvertexShader);
	glDeleteShader(effectvertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(effectfragmentShader);
	glUseProgram(shaderID);
	glUseProgram(EffectShaderID);
}
using namespace std;



