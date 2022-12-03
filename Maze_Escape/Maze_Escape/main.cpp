#include "stdafx.h"
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
#include <mmsystem.h>    
#include<Windows.h>
#pragma comment(lib,"winmm.lib")

#include "filetobuf.h"
#include "MakeShader.h"
#include "readobj.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void InitBuffer();
void initTexture();
using namespace std;
unsigned int texture[10];
//call_back
void MainView();
void Resize(int w, int h);
void keyboard(unsigned char key, int x, int y);

//func
void timer(int value);
void ObjList();
void drawscene();

int WIDTH = 0;
int HEIGHT = 0;

//Struck

//--snow
struct Snow {
	float x;
	float y;
	float z;
};
Snow SLocation[200];
Snow SaveLocation[200];
Snow SnowSpeed[200];


//--Light cube
struct Star {
	float x;
	float y;
	float z;
};
Star StarLocation[80];
Star StarSaveLocation[80];
Star StarSpeed[80];

struct Camera
{
	float C_x = 1.0f;
	float C_y = 5.0f;
	float C_z = 9.0f;

}Camerapos;

struct Angle
{
	float angle = 0.0f;
	float anglex = 0.0f;
	float angley = 0.0f;
	float AngleTrap2 = 00.0f;
	float AngleTrap = 0.0f;
	float Radian = 0.0f;
	//---------------------
	float AngleRevolx = 45.0f;
	float AngleRevoly = 45.0f;
	//----------------------
	float anglecamera = 160.0f;
	float anglecamera2 = 0.0f;
	float ObjAngle = 20.0f;
	//------------------
	float LightRadian = 10.0f;
	//------------------
	float BodyAngle = 0.0f;
	float ArmAngle = 45.0f;
	float LegAngle = 0.0f;
	float MainSwing1 = 0.0f;
	float MainSwing2 = 0.0f;
	float StageAngle = 0.0f;
	float StageAngle2 = 0.0f;
	float EyeAngle = 0.0f;
	float CpaeAngle = 0.0f;
}AngleList;

//scale
struct Scale
{
	float X = 0.0f;
	float Y = 4.5f;
	float Z = 6.5f;
	//-------------
	float x = 2.0f;
	float y = 2.0f;
	float z = 2.0f;
	//-------------
	float Mx = 0.0f;
	float My = 0.0f;
	float Mz = 0.0f;
	//--------------
	float ALx = 0.1f;
	float ALy = 0.3f;
	float ALz = 0.1f;
	//---------------
	float TSy = 0.6f;
	//---------------
	float Doorx = 6.0f;
	//---------------
	float PullScaleRx[3] = { 6.0f,6.0f,6.0f };
	float PullScaleRz = 2.0f;
	float PullScaleRy = 2.0f;
	//------------------
	float PullScaleLx[3] = { 6.0f,6.0f,6.0f };
	float PullScaleLz = 2.0f;
	float PullScaleLy = 2.0f;
}Scalepos;

//trans
struct Transration
{
	float T_x = 0.0f;
	float T_y = 0.8f;
	float T_z = 0.0f;
	//---------------
	float T_StageX = 0.0f;
	float T_StageY = 0.0f;
	float T_StageZ = 0.0f;
	//----------------
	float T_Stage2X = 0.0f;
	float T_Stage2Y = 0.0f;
	float T_Stage2Z = 0.0f;
	//------------------
	float T_Stage2ZA[3] = { 35.0f,50.0f,65.0f };
	//----------------
	float T_Trapx = 0.0f;
	float T_Trapy = 0.6f;
	float T_Trapz = 0.0f;
	//-----------------
	float T_Trapx2 = 16.0f;
	float T_Trapy2 = 1.5f;
	float T_Trapz2 = 20.0f;
	//---------------
	float T_Bodyx = 0.0f;
	float T_Bodyy = 0.9f;
	float T_Bodyz = -2.0f;
	//--------------------
	float T_Cpaex = 0.0f;
	float T_Cpaey = 0.6f;
	float T_Cpaez = -2.0f;
	//-------------------
	float T_Eyex = 0.0f;
	float T_Eyey = 0.9f;
	float T_Eyez = -2.0f;
	//----------------
	float T_ArmLegx = 0.0f;
	float T_ArmLegy = 0.77f;
	float T_ArmLegz = 0.0f;
	//-------------------
	float DoorxL = 1.8f;
	float DoorxR = -1.8f;

}TransList;


GLuint VAO[30];
GLuint VBO[90];

vector<glm::vec4> Vertex[26];
vector<glm::vec4> Nomal[26];
vector<glm::vec2> Texture[26];


int Mainswingchk = 1;

glm::vec3 objC = glm::vec3(0, 0, 0);
glm::vec3 cameraPos = glm::vec3(1.0f, 3.0f, 10.0f);
glm::vec3 lightPos = glm::vec3(0, 3.0f, 2.5f);
glm::vec3 lightColor = glm::vec3(1.4f, 1.3f, 1.3f);
glm::vec3 Cameraposdir = glm::vec3(0.0f);
glm::vec3 Cameradir = glm::vec3(0.0f);
float Movevalue = 0.2f;

int main(int argc, char** argv)
{
	//PlaySound(TEXT("backsound.wav"), NULL, SND_ASYNC | SND_ALIAS);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Maze_Escape");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		cerr << "fail Initialize" << endl;
	else cout << "Initialize" << endl;
	ObjList();

	makeShaderID();
	InitBuffer();
	initTexture();
	glutDisplayFunc(MainView);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1, timer, 1);
	glutMainLoop();
}

void timer(int value)
{	
	glutPostRedisplay();
	glutTimerFunc(17, timer, value);
}

void MainView()
{
	glClearColor(0.9989, 0.9989, 0.9989, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	{
		glBindVertexArray(VAO[0]);
		unsigned int StartGroundBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(StartGroundBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
		glm::mat4 StartGround = glm::mat4(1.0f);
		unsigned int StartGroundNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(StartGroundNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		unsigned int cameraViewLocation = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(cameraViewLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		unsigned int cameraPosLocation = glGetUniformLocation(shaderID, "cameraPos");
		glUniform3fv(cameraPosLocation, 1, glm::value_ptr(glm::vec3(0, 0, 1)));
		unsigned int projectionLocation = glGetUniformLocation(shaderID, "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		StartGround = glm::translate(StartGround, glm::vec3(0, 0, -1));
		StartGround = glm::rotate(StartGround, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		StartGround = glm::scale(StartGround, glm::vec3(2, 2, 2));
		unsigned int backgroundMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(backgroundMatrixLocation, 1, GL_FALSE, glm::value_ptr(StartGround));
		glDrawArrays(GL_TRIANGLES, 0, Vertex[0].size());
	}

	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glUseProgram(shaderID);

	glm::vec3 ObjectCamerapos = glm::vec3(Camerapos.C_x - TransList.T_Bodyx, Camerapos.C_y + TransList.T_Bodyy, Camerapos.C_z - TransList.T_Bodyz);
	glm::mat4 CameraSpacepos = glm::mat4(1.0f);
	CameraSpacepos = glm::rotate(CameraSpacepos, glm::radians(AngleList.anglecamera), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 Cameraposdir = glm::vec3(CameraSpacepos * glm::vec4(ObjectCamerapos, 1));
	glm::vec3 ObjectCameraPicking = glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy, TransList.T_Bodyz);
	glm::vec3 ObjectCameradir = Cameraposdir - ObjectCameraPicking;
	glm::vec3 Up_y = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 CameraSpacedir = glm::mat4(1.0f);
	CameraSpacedir = glm::rotate(CameraSpacedir, glm::radians(AngleList.anglecamera2), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 Cameradir = glm::vec3(CameraSpacedir * glm::vec4(-ObjectCameradir, 1));

	glm::vec3 Crosspos = glm::normalize(glm::cross(Up_y, glm::normalize(Cameradir)));
	glm::vec3 Cross = glm::cross(glm::normalize(Cameradir), Crosspos);

	glm::mat4 ObjView = glm::mat4(1.0f);
	ObjView = glm::lookAt(Cameraposdir, Cameradir + Cameraposdir, Cross);
	unsigned int ObjcameraViewLocation = glGetUniformLocation(shaderID, "viewTransform");
	glUniformMatrix4fv(ObjcameraViewLocation, 1, GL_FALSE, glm::value_ptr(ObjView));

	glm::vec3 CameraView;
	CameraView = glm::vec3(Cameraposdir);
	unsigned int ViewPositionLocation = glGetUniformLocation(shaderID, "camerapos");
	glUniform3fv(ViewPositionLocation, 1, glm::value_ptr(CameraView));

	glm::mat4 Proj = glm::mat4(1.0f);
	Proj = glm::perspective(glm::radians(60.0f), (float)WIDTH / HEIGHT, 0.1f, 200.0f);
	unsigned int ModelProjLocation = glGetUniformLocation(shaderID, "projectionTransform");
	glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &Proj[0][0]);

	glm::mat4 LightPosition = glm::mat4(1.0f);
	LightPosition = glm::rotate(LightPosition, glm::radians(AngleList.LightRadian), glm::vec3(0.0f, 1.0f, 0.0f));
	LightPosition = glm::translate(LightPosition, glm::vec3(Scalepos.X, Scalepos.Y, Scalepos.Z - 1.0f));
	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "LightPos");
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "LightColor");
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
	unsigned int LightTransformLocation = glGetUniformLocation(shaderID, "LightTransform");
	glUniformMatrix4fv(LightTransformLocation, 1, GL_FALSE, glm::value_ptr(LightPosition));


	drawscene();
	glutSwapBuffers();
}

void Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'w':
		Mainswingchk = 1;

		TransList.T_Bodyz += Movevalue;

		break;
	case's':
		Mainswingchk = 1;

		TransList.T_Bodyz -= Movevalue;

		break;
	case'a':
		Mainswingchk = 2;

		TransList.T_Bodyx += Movevalue;

		break;
	case'd':
		Mainswingchk = 2;

		TransList.T_Bodyx -= Movevalue;

		break;
	case'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void InitBuffer()
{
	glGenVertexArrays(26, VAO);

	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(VAO[i]);
		glGenBuffers(3, &VBO[3 * i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Vertex[i].size(), &Vertex[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i + 1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * Nomal[i].size(), &Nomal[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3 * i + 2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * Texture[i].size(), &Texture[i][0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(2);

	}

}

void ObjList()
{
	//readTriangleObj("plane3.obj", Vertex[0], Texture[0], Nomal[0]);
	//readTriangleObj("plane3.obj", Vertex[9], Texture[9], Nomal[9]);


	//readTriangleObj("cube3_.obj", Vertex[2], Texture[2], Nomal[2]);
	//readTriangleObj("Sphere2.obj", Vertex[3], Texture[3], Nomal[3]);
	readTriangleObj("plane3.obj", Vertex[0], Texture[0], Nomal[0]);
	readTriangleObj("cube.obj", Vertex[1], Texture[1], Nomal[1]);
	readTriangleObj("cube.obj", Vertex[2], Texture[2], Nomal[2]);
	readTriangleObj("Sphere.obj", Vertex[3], Texture[3], Nomal[3]);
	//readTriangleObj("Sphere2.obj", Vertex[1], Texture[1], Nomal[1]);
	//for (int i = 5; i < 9; i++)
	//{
	//	readTriangleObj("Sphere2.obj", Vertex[i], Texture[i], Nomal[i]);
	//}
	//for (int i = 10; i < 13; i++)
	//{
	//	readTriangleObj("Sphere2.obj", Vertex[i], Texture[i], Nomal[i]);
	//}

	//readTriangleObj("hely.obj", Vertex[13], Texture[13], Nomal[13]);
	//readTriangleObj("hely.obj", Vertex[14], Texture[14], Nomal[14]);
	//readTriangleObj("cube3_.obj", Vertex[15], Texture[15], Nomal[15]);
	//readTriangleObj("cube3_.obj", Vertex[16], Texture[16], Nomal[16]);
	//readTriangleObj("hely.obj", Vertex[17], Texture[17], Nomal[17]);
	//readTriangleObj("crown.obj", Vertex[18], Texture[18], Nomal[18]);
	//readTriangleObj("sword.obj", Vertex[19], Texture[19], Nomal[19]);

	//readTriangleObj("plane3.obj", Vertex[20], Texture[20], Nomal[20]);
	//readTriangleObj("cube3_.obj", Vertex[21], Texture[21], Nomal[21]);
	//readTriangleObj("cube3_.obj", Vertex[22], Texture[22], Nomal[22]);
	//readTriangleObj("cube3_.obj", Vertex[23], Texture[23], Nomal[23]);
	//readTriangleObj("fallingice.obj", Vertex[24], Texture[24], Nomal[24]);
}

void drawscene()
{
	glUseProgram(shaderID);

	/*for (int i = 0; i < 200; i++)
	{
		glBindVertexArray(VAO[3]);
		unsigned int snowBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(snowBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glm::mat4 Snow = glm::mat4(1.0f);
		Snow = glm::translate(Snow, glm::vec3(SLocation[i].x, SLocation[i].y, SLocation[i].z));
		Snow = glm::scale(Snow, glm::vec3(0.03f, 0.03f, 0.03f));
		unsigned int SnowLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(SnowLocation, 1, GL_FALSE, glm::value_ptr(Snow));
		glm::mat4 SnowNormalmodel = glm::mat4(1.0f);
		SnowNormalmodel = glm::translate(SnowNormalmodel, glm::vec3(SLocation[i].x, SLocation[i].y, SLocation[i].z));
		unsigned int SnowNormalmodelLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(SnowNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(SnowNormalmodel));
		unsigned int SnowColorLocation = glGetUniformLocation(shaderID, "objColor");
		glUniform3f(SnowColorLocation, 1, 0.388235, 0.278431);
		glDrawArrays(GL_TRIANGLES, 0, Vertex[3].size());
	}


	for (int i = 0; i < 80; i++)
	{
		glBindVertexArray(VAO[23]);
		unsigned int StarBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(StarBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glm::mat4 Star = glm::mat4(1.0f);
		Star = glm::translate(Star, glm::vec3(StarLocation[i].x, StarLocation[i].y, StarLocation[i].z));
		Star = glm::scale(Star, glm::vec3(2.0f, 2.0f, 2.0f));
		unsigned int StarLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(StarLocation, 1, GL_FALSE, glm::value_ptr(Star));
		glm::mat4 StarNormalmodel = glm::mat4(1.0f);
		StarNormalmodel = glm::translate(StarNormalmodel, glm::vec3(SLocation[i].x, SLocation[i].y, SLocation[i].z));
		unsigned int StarNormalmodelLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(StarNormalmodelLocation, 1, GL_FALSE, glm::value_ptr(StarNormalmodel));
		unsigned int StarColorLocation = glGetUniformLocation(shaderID, "objColor");
		glUniform3f(StarColorLocation, 1, 0.388235, 0.278431);
		glDrawArrays(GL_TRIANGLES, 0, Vertex[23].size());
	}*/

	glBindVertexArray(VAO[0]);	// 바닥
	unsigned int StageBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(StageBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	glm::mat4 StageTrasMatrix = glm::mat4(1.0f);
	StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(0.f, 0.f, 50.f));
	//StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(70.f), glm::vec3(0.0f, 1.0f, 0.0f));
	StageTrasMatrix = glm::scale(StageTrasMatrix, glm::vec3(100.0, 1.0, 100.0));
	unsigned int StageTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(StageTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageTrasMatrix));
	glm::mat4 StageNormalMatrix = glm::mat4(1.0f);
	unsigned int StageNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(StageNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[0].size());

	for (int i = 0; i < 11; ++i)
	{
		glBindVertexArray(VAO[1]);	// 미로
		unsigned int mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(mazeBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
		glm::mat4 mazeTrasMatrix = glm::mat4(1.0f);
		//mazeTrasMatrix = glm::rotate(mazeTrasMatrix, glm::radians(70.f), glm::vec3(0.0f, 1.0f, 0.0f));
		mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(-50.f, 10.f, 5.f + i * 10.f));
		mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
		unsigned int mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
		glm::mat4 mazeNormalMatrix = glm::mat4(1.0f);
		unsigned int mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
		glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

		glBindVertexArray(VAO[1]);	// 미로
		mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(mazeBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
		mazeTrasMatrix = glm::mat4(1.0f);
		//mazeTrasMatrix = glm::rotate(mazeTrasMatrix, glm::radians(70.f), glm::vec3(0.0f, 1.0f, 0.0f));
		mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(-50.f + i * 10.f, 10.f, 100.f ));
		mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
		mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
		mazeNormalMatrix = glm::mat4(1.0f);
		mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
		glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

		glBindVertexArray(VAO[1]);	// 미로
		mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
		glUniform1i(mazeBlendCheck, 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
		mazeTrasMatrix = glm::mat4(1.0f);
		//mazeTrasMatrix = glm::rotate(mazeTrasMatrix, glm::radians(70.f), glm::vec3(0.0f, 1.0f, 0.0f));
		mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(50.f, 10.f, 5.f + i * 10.f));
		mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
		mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
		glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
		mazeNormalMatrix = glm::mat4(1.0f);
		mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
		glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
		glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());
	}
	
#pragma region 플레이어
	glBindVertexArray(VAO[1]);	// 플레이어 몸통
	unsigned int playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	glm::mat4 playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(1.0, 1.3, 1.0));
	unsigned int playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	glm::mat4 playerNormalMatrix = glm::mat4(1.0f);
	unsigned int playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

	glBindVertexArray(VAO[2]);	// 플레이어 팔
	playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.4f, 0.7f, 0.4f));
	playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	playerNormalMatrix = glm::mat4(1.0f);
	playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

	glBindVertexArray(VAO[2]);	// 플레이어 팔
	playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.4f, 0.7f, 0.4f));
	playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	playerNormalMatrix = glm::mat4(1.0f);
	playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

	glBindVertexArray(VAO[2]);	// 플레이어 다리
	playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.35f, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.3f, 0.8f, 0.3f));
	playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	playerNormalMatrix = glm::mat4(1.0f);
	playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

	glBindVertexArray(VAO[2]);	// 플레이어 다리
	playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.35f, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.3f, 0.8f, 0.3f));
	playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	playerNormalMatrix = glm::mat4(1.0f);
	playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

	glBindVertexArray(VAO[3]);	// 플레이어 머리
	playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
	glUniform1i(playerBlendCheck, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
	playerTrasMatrix = glm::mat4(1.0f);
	playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.7f, TransList.T_Bodyz));
	playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
	playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
	playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
	glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
	playerNormalMatrix = glm::mat4(1.0f);
	playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
	glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
	glDrawArrays(GL_TRIANGLES, 0, Vertex[3].size());

	

#pragma endregion 플레이어
}


void initTexture()
{
	glGenTextures(9, &texture[0]);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int TailWidthImage, TailHeightImage, TailnumberOfChannel;
	unsigned char* TailData = stbi_load("Texture/tail.jpg", &TailWidthImage, &TailHeightImage, &TailnumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TailWidthImage, TailHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, TailData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(TailData);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int mazeWidthImage, mazeHeightImage, mazenumberOfChannel;
	unsigned char* mazeData = stbi_load("Texture/Punchimage.jpg", &mazeWidthImage, &mazeHeightImage, &mazenumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mazeWidthImage, mazeHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, mazeData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(mazeData);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int IceWidthImage, IceHeightImage, IcenumberOfChannel;
	unsigned char* IceData = stbi_load("Texture/Hely.jpg", &IceWidthImage, &IceHeightImage, &IcenumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IceWidthImage, IceHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, IceData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(IceData);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int fireWidthImage, fireHeightImage, firenumberOfChannel;
	unsigned char* fireData = stbi_load("Texture/fireball.jpg", &fireWidthImage, &fireHeightImage, &firenumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fireWidthImage, fireHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, fireData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(fireData);

	/*glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int  PotalWidthImage, PotalHeightImage, PotalnumberOfChannel;
	unsigned char* PotalData = stbi_load("Texture/Potalimage.jpg", &PotalWidthImage, &PotalHeightImage, &PotalnumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PotalWidthImage, PotalHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, PotalData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(PotalData);

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int  PunchWidthImage, PunchHeightImage, PunchnumberOfChannel;
	unsigned char* PunchData = stbi_load("Texture/Punchimage.jpg", &PunchWidthImage, &PunchHeightImage, &PunchnumberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PunchWidthImage, PunchHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, PunchData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(PunchData);*/

}