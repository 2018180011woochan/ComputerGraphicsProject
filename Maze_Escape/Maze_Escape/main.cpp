#include "stdafx.h"

#include <random>
#include "readobj.h"
#include "CGameObject.h"
#include "CPlayer.h"

using namespace std;

random_device rd;
default_random_engine dr(rd());
uniform_real_distribution<> uid(0.1, 2.5);
uniform_real_distribution<> randColor(0.0, 1.0);

vector<glm::vec4> ObjVertexVal[2];
float randomY[25][25];
float RandomColor[25][25];

GLuint VAO[2], VBO[3];
GLuint shaderID[2];
GLuint qobjshader;
GLuint vertexshader;
GLuint fragmentshader;

void initbuffer();
void make_vertexShaders();
void make_fragmentShaders();
void timer(int value);
void Resize(int w, int h);
GLvoid drawscene();
char* filetobuf(const string name);
GLuint make_shaderProgram();
GLvoid Reshape(int Width, int Height);
GLvoid keyboard(unsigned char key, int x, int y);
void MySpecialKey(int Key, int X, int Y);
GLvoid ViewPort();

void MakeMaze();
void Up();
void Down();
void Left();
void Right();
void PlayerUp();
void PlayerDown();
void PlayerLeft();
void PlayerRight();

int width, height;

int MoveSpeed = 10;
int curWidth = 0;
int curHeight = 0;

bool MoveDir = false;
int moveCnt = 0;

int WidthInput, HeightInput;

int PlayerXPos = 1;
int PlayerZPos = 0;
float PlayerX = 0.f;
float PlayerZ = 0.f;
float startX, startZ;

bool dlscld1 = false;
bool dlscld3 = true;

struct ObjTrans
{
	float X = 0.0f;
	float Z = 0.0f;
	float Y = 5.0f;
}TransList;

struct ObjRot
{
	float X = 0.0f;
	float Z = 0.0f;
	float Y = 0.0f;
	float CameraY1 = 0.0f;
	float CameraY2 = 0.0f;
}RotList;

struct Camera
{
	float C_x = 0.0f;
	float C_y = 5.f;
	float C_z = 11.0f;

}CameraPos;

CGameObject* _player;

GLvoid Reshape(int Width, int Height)
{
	glViewport(0, 0, 1600, 900);
}

void Resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}

void make_vertexShaders() {
	GLchar* vertexsource1;

	vertexsource1 = filetobuf("vertex.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vertexsource1, NULL);
	glCompileShader(vertexshader);

	GLchar* vertexsource2;

	vertexsource2 = filetobuf("vertex.glsl");

	qobjshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(qobjshader, 1, &vertexsource2, NULL);
	glCompileShader(qobjshader);

	GLint result;
	GLchar errorlog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexshader, 512, NULL, errorlog);
		cerr << "Error:vertexshader 컴파일 실패" << errorlog << endl;
	}

}

void make_fragmentShaders() {
	GLchar* fragmentsource;
	fragmentsource = filetobuf("fragment.glsl");

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fragmentsource, NULL);
	glCompileShader(fragmentshader);

	GLint result;
	GLchar errorlog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorlog);
		cerr << "Error:fragmentshader 컴파일 실패" << errorlog << endl;
	}

}

GLuint make_shaderProgram()
{
	shaderID[0] = glCreateProgram();

	glAttachShader(shaderID[0], vertexshader);
	glAttachShader(shaderID[0], fragmentshader);

	shaderID[1] = glCreateProgram();

	glAttachShader(shaderID[1], qobjshader);
	glAttachShader(shaderID[1], fragmentshader);

	glLinkProgram(shaderID[0]);
	glLinkProgram(shaderID[1]);

	glDeleteShader(vertexshader);
	glDeleteShader(qobjshader);
	glDeleteShader(fragmentshader);

	glUseProgram(shaderID[0]);
	glUseProgram(shaderID[1]);

	return 1;
}

char* filetobuf(const string name)
{
	vector<char> tempFile;
	ifstream in(name, ios::binary);
	char temp;
	while (true) {
		noskipws(in);
		in >> temp;
		if (in.eof()) {
			tempFile.push_back(0);
			break;
		}
		else
			tempFile.push_back(temp);
	}
	char* addr = new char[tempFile.size()];
	for (int i = 0; i < tempFile.size(); i++) {
		addr[i] = tempFile[i];
	}
	return addr;
}

glm::vec3 cameraPos;
GLvoid ViewPort()
{
	glClearColor(0.2, 0.2, 0.2, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderID[0]);

	glViewport(0, 0, width, (height / 4) * 3);

	if (dlscld3)
		cameraPos = glm::vec3(CameraPos.C_x, CameraPos.C_y, CameraPos.C_z);
	if (dlscld1)
		cameraPos = glm::vec3(_player->_xPos, 1.f, _player->_zPos);

	glm::mat4 CameraSpacePos = glm::mat4(1.0f);
	CameraSpacePos = glm::rotate(CameraSpacePos, glm::radians(RotList.CameraY1), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 CameraPosDistance = glm::vec4(cameraPos, 1);
	CameraPosDistance = CameraSpacePos * CameraPosDistance;
	glm::vec3 CameraPosDir = glm::vec3(CameraPosDistance.x, CameraPosDistance.y, CameraPosDistance.z);
	glm::vec3 ObjectCameraPicking = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ObjectCameradir = CameraPosDir - ObjectCameraPicking;
	glm::vec3 Up_y = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 CameraSpacedir = glm::mat4(1.0f);
	CameraSpacedir = glm::rotate(CameraSpacedir, glm::radians(RotList.CameraY2), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 CameraSpaceMul = CameraSpacedir * glm::vec4(ObjectCameradir, 1);
	glm::vec3 Cameradir;
	Cameradir.x = CameraSpaceMul.x; Cameradir.y = CameraSpaceMul.y; Cameradir.z = CameraSpaceMul.z;
	Cameradir = glm::normalize(Cameradir);

	glm::mat4 RotateSpacedir = glm::mat4(1.0f);
	RotateSpacedir = glm::rotate(RotateSpacedir, glm::radians(RotList.CameraY2), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 RotateobjSpacedir = glm::vec3(ObjectCameraPicking - CameraPosDir);
	glm::vec4 Rotatedir = glm::vec4(RotateobjSpacedir, 1);
	Rotatedir = RotateSpacedir * Rotatedir;
	RotateobjSpacedir = glm::vec3(Rotatedir.x + CameraPosDir.x, Rotatedir.y + CameraPosDir.y, Rotatedir.z + CameraPosDir.z);
	glm::vec3 Crosspos = glm::normalize(glm::cross(Up_y, Cameradir));
	glm::vec3 Cross = glm::cross(Cameradir, Crosspos);

	glm::mat4 S_proj = glm::mat4(1.0f);
	S_proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
	unsigned int ProjLocation = glGetUniformLocation(shaderID[0], "projectiontransform");
	glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, &S_proj[0][0]);


	glm::mat4 S_View = glm::mat4(1.0f);
	S_View = glm::lookAt(CameraPosDir, RotateobjSpacedir, Cross);
	unsigned int ViewLocation = glGetUniformLocation(shaderID[0], "viewtransform");
	glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(S_View)); //뷰


//------------------------------camera---------------------------------------------
	
	//glm::vec3 ObjectCamerapos = glm::vec3(CameraPos.C_x - _player->_xPos, CameraPos.C_y + _player->_yPos, CameraPos.C_z - _player->_zPos);
	//glm::vec3 ObjectCamerapos = glm::vec3(_player->_xPos, _player->_yPos, _player->_zPos);
	//glm::mat4 CameraSpacepos = glm::mat4(1.0f);
	//CameraSpacepos = glm::rotate(CameraSpacepos, glm::radians(160.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::vec3 Cameraposdir = glm::vec3(CameraSpacepos * glm::vec4(ObjectCamerapos, 1));
	//glm::vec3 ObjectCameraPicking = glm::vec3(_player->_xPos, _player->_yPos, _player->_zPos);
	//glm::vec3 ObjectCameradir = Cameraposdir - ObjectCameraPicking;
	//glm::vec3 Up_y = glm::vec3(0.0f, 1.0f, 0.0f);

	//glm::mat4 CameraSpacedir = glm::mat4(1.0f);
	//CameraSpacedir = glm::rotate(CameraSpacedir, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::vec3 Cameradir = glm::vec3(CameraSpacedir * glm::vec4(-ObjectCameradir, 1));

	//glm::vec3 Crosspos = glm::normalize(glm::cross(Up_y, glm::normalize(Cameradir)));
	//glm::vec3 Cross = glm::cross(glm::normalize(Cameradir), Crosspos);

	//glm::mat4 ObjView = glm::mat4(1.0f);
	//ObjView = glm::lookAt(Cameraposdir, Cameradir + Cameraposdir, Cross);
	//unsigned int ObjcameraViewLocation = glGetUniformLocation(shaderID[0], "viewtransform");
	//glUniformMatrix4fv(ObjcameraViewLocation, 1, GL_FALSE, glm::value_ptr(ObjView));

	//glm::vec3 CameraView;
	//CameraView = glm::vec3(Cameraposdir);
	//unsigned int ViewPositionLocation = glGetUniformLocation(shaderID[0], "camerapos");
	//glUniform3fv(ViewPositionLocation, 1, glm::value_ptr(CameraView));

	////--------����
	//glm::mat4 Proj = glm::mat4(1.0f);
	//Proj = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 100.0f);
	//unsigned int ModelProjLocation = glGetUniformLocation(shaderID[0], "projectionTransform");
	//glUniformMatrix4fv(ModelProjLocation, 1, GL_FALSE, &Proj[0][0]);

	drawscene();



	glViewport((width / 4) * 3, (height / 4) * 3, width / 4, height / 4);
	glm::vec3 PlainCameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 PlainCameraDirection = glm::vec3(1.0f, 0.0f, 0.0f) - PlainCameraPos;
	glm::vec3 PlainViewUp = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 PlainView = glm::mat4(1.0f);
	glm::mat4 PlainOrtho = glm::mat4(1.0f);
	unsigned int PlainViewLocation = glGetUniformLocation(shaderID[1], "viewtransform");
	unsigned int PlainOrthoLocation = glGetUniformLocation(shaderID[1], "projectiontransform");
	PlainView = glm::lookAt(PlainCameraPos, PlainCameraDirection, PlainViewUp);
	PlainOrtho = glm::ortho(-2.0f, 2.0f, -1.0f, 2.0f, 0.5f, 36.0f);
	glUniformMatrix4fv(PlainViewLocation, 1, GL_FALSE, glm::value_ptr(PlainView));
	glUniformMatrix4fv(PlainOrthoLocation, 1, GL_FALSE, glm::value_ptr(PlainOrtho));
	drawscene();

	glutSwapBuffers();
}

GLvoid drawscene()
{
	glUseProgram(shaderID[0]);

	glBindVertexArray(VAO[0]);	// 바닥
	glm::mat4 Floor = glm::mat4(1.0f);
	Floor = glm::scale(Floor, glm::vec3(0.5f * WidthInput, 1.f, 0.5f * HeightInput));
	Floor = glm::translate(Floor, glm::vec3(0.0f, 0.0f, 0.0f));
	unsigned int ObjFloorLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(ObjFloorLocation, 1, GL_FALSE, glm::value_ptr(Floor));
	unsigned int ObjFloorfragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(ObjFloorfragLocation, 1.f, 1.f, 1.f);
	glDrawArrays(GL_QUADS, 0, 24);
	startX = 0.f - 0.5f * WidthInput;
	startZ = 0.f - 0.5f * HeightInput;

	float realStartX = startX + 0.5f;
	float realStartZ = startZ + 0.5f;

	for (int i = 0; i < WidthInput; ++i)
	{
		for (int j = 0; j < HeightInput; ++j)
		{
			glBindVertexArray(VAO[1]);	// 기둥
			glm::mat4 Pillar = glm::mat4(1.0f);
			Pillar = glm::scale(Pillar, glm::vec3(0.5f, randomY[i][j], 0.5f));
			Pillar = glm::translate(Pillar, glm::vec3(realStartX + i * 1.0f, TransList.Y, realStartZ + j * 1.0f));
			unsigned int ObjPillarLocation = glGetUniformLocation(shaderID[1], "modeltransform");
			glUniformMatrix4fv(ObjPillarLocation, 1, GL_FALSE, glm::value_ptr(Pillar));
			unsigned int ObjPillarfragLocation = glGetUniformLocation(shaderID[1], "vColor");
			glUniform3f(ObjPillarfragLocation, RandomColor[i][j], RandomColor[j][i], RandomColor[i][j]);
			glDrawArrays(GL_QUADS, 0, 24);
		}
	}

	PlayerX = realStartX + 1.0f;
	PlayerZ = realStartZ;

	glBindVertexArray(VAO[2]);	// 플레이어
	glm::mat4 Player = glm::mat4(1.0f);
	Player = glm::scale(Player, glm::vec3(0.2f, 0.3f, 0.2f));
	Player = glm::translate(Player, glm::vec3(PlayerX + _player->GetPlayerXPos(), 0.f, PlayerZ + _player->GetPlayerZPos()));
	unsigned int ObjPlayerLocation = glGetUniformLocation(shaderID[1], "modeltransform");
	glUniformMatrix4fv(ObjPlayerLocation, 1, GL_FALSE, glm::value_ptr(Player));
	unsigned int ObjPlayerfragLocation = glGetUniformLocation(shaderID[1], "vColor");
	glUniform3f(ObjPlayerfragLocation, 1.f, 0.f, 0.f);
	glDrawArrays(GL_QUADS, 0, 24);


}

void initbuffer()
{
	glGenVertexArrays(2, VAO);

	glBindVertexArray(VAO[0]);	// 바닥
	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ObjVertexVal[0].size(), &ObjVertexVal[0][0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);	// 기둥
	glGenBuffers(1, &VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ObjVertexVal[1].size(), &ObjVertexVal[1][0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[2]);	// 플레이어
	glGenBuffers(1, &VBO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * ObjVertexVal[1].size(), &ObjVertexVal[1][0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	_player = new CPlayer();
	MakeMaze();
}

void timer(int value)
{
	if (TransList.Y >= 0.f)
		TransList.Y -= 0.1f;



	glutPostRedisplay();
	glutTimerFunc(MoveSpeed, timer, 1);

}

void MakeMaze()
{
	int dir;
	while (1)
	{
		if (curHeight == HeightInput - 1/*  || curWidth == WidthInput - 1*/)
		{
			break;
		}

		// 방향 정하기
		if (MoveDir)
		{
			dir = rand() % 2;
			MoveDir = false;
		}
		else
		{
			dir = rand() % 2 + 2;
			MoveDir = true;
		}
		// 칸수 정하기
		moveCnt = rand() % 7 + 2;

		// 상하라면
		switch (dir)
		{
		case 0:		// 우
			if (curHeight >= HeightInput - 1)
			{
				Left();
				break;
			}

			Right();
			break;
		case 1:		// 좌
			if (curHeight <= 0)
			{
				Right();
				break;
			}
			Left();
			break;
		case 2:		// 상
			if (curWidth <= 0)
			{
				Down();
				break;
			}
			Up();
			break;
		case 3:		// 하
			if (curWidth >= WidthInput - 1)
			{
				Up();
				break;
			}
			Down();

			break;
		}

	}
}

void Up()
{
	for (int i = 0; i < moveCnt; ++i)
	{
		if (curWidth <= 0)
		{
			curWidth = 0;
			break;
		}

		curWidth--;
		randomY[curWidth][curHeight] = 0.f;
	}
}

void Down()
{
	for (int i = 0; i < moveCnt; ++i)
	{
		if (curWidth >= WidthInput - 1)
		{
			curWidth = WidthInput - 1;
			break;
		}

		curWidth++;
		randomY[curWidth][curHeight] = 0.f;
	}
}

void Left()
{
	for (int i = 0; i < moveCnt; ++i)
	{
		if (curHeight <= 0) {
			curHeight = 0;
			break;
		}

		curHeight--;
		randomY[curWidth][curHeight] = 0.f;
	}
}

void Right()
{
	for (int i = 0; i < moveCnt; ++i)
	{
		if (curHeight >= HeightInput - 1)
		{
			curHeight = HeightInput - 1;
			break;
		}

		curHeight++;
		randomY[curWidth][curHeight] = 0.f;
	}
}

GLvoid keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		dlscld1 = true;
		dlscld3 = false;
		break;
	case '3':
		dlscld3 = true;
		dlscld1 = false;
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void PlayerUp()
{
	//if (randomY[PlayerXPos][PlayerZPos - 1] == 0.f)
	{
		_player->SetPlayerZPos(_player->GetPlayerZPos() - 1.f);
		PlayerZPos -= 1;
	}
}

void PlayerDown()
{
	//if (randomY[PlayerXPos][PlayerZPos + 1] == 0.f)
	{
		_player->SetPlayerZPos(_player->GetPlayerZPos() + 1.f);
		PlayerZPos += 1;
	}
}

void PlayerLeft()
{
	//if (randomY[PlayerXPos - 1][PlayerZPos] == 0.f)
	{
		_player->SetPlayerXPos(_player->GetPlayerXPos() - 1.f);
		PlayerXPos -= 1;
	}
}

void PlayerRight()
{
	//if (randomY[PlayerXPos + 1][PlayerZPos] == 0.f)
	{
		_player->SetPlayerXPos(_player->GetPlayerXPos() + 1.f);
		PlayerXPos += 1;
	}
}

void MySpecialKey(int Key, int X, int Y)
{
	switch (Key) {
	case GLUT_KEY_LEFT:     //왼쪽 키
		PlayerLeft();
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:     //오른쪽 키
		PlayerRight();
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:      //위 키
		PlayerUp();
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:      //아래 키
		PlayerDown();
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1600, 900);
	glutCreateWindow("Moving Mountain & Maze");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		cerr << "fail Initialize" << endl;

	cout << "**********************************************************************************\n";
	cout << "o/p 투영을 선택한다 (직각 투영 / 원근 투영\n";
	cout << "z/Z 원근 투영 시 z축으로 이동할 수 있게 한다\n";
	cout << "m/M: 육면체들이 위 아래로 움직인다 / 멈춘다\n";
	cout << "y/Y: 카메라가 바닥의 y축을 기준으로 양/음 방향으로 회전한다.\n";
	cout << "r: 미로를 제작한다\n";
	cout << "v: 육면체들 움직임이 멈추고 낮은 높이로 변한다, 다시 누르면 움직임이 다시 시작된다\n";
	cout << "s: 미로에서 객체가 나타난다\n";
	cout << "→/←/↑/↓: 객체를 앞/뒤/좌/우 이동\n";
	cout << "+/-: 육면체 이동하는 속도 증가/감소\n";
	cout << "1/3: 카메라 시점 1인칭/3인칭 변환\n";
	cout << "c: 모든 값 초기화\n";
	cout << "q: 프로그램 종료\n";
	cout << "**********************************************************************************\n";
	//cout << "가로 : ";
	//cin >> WidthInput;
	//cout << "세로 : ";
	//cin >> HeightInput;

	WidthInput = 25;
	HeightInput = 25;

	for (int i = 0; i < WidthInput; ++i)
	{
		for (int j = 0; j < HeightInput; ++j)
		{
			float RandomY = uid(dr);
			//randomY[i][j] = RandomY;
			randomY[i][j] = 0.1f;
			RandomColor[i][j] = randColor(dr);
		}
	}

	readobj("square1.obj", ObjVertexVal[0]);		// 직육면체 면
	readobj("cube.obj", ObjVertexVal[1]);			// 사각형

	make_vertexShaders();
	make_fragmentShaders();
	make_shaderProgram();
	initbuffer();
	glutDisplayFunc(ViewPort);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(MySpecialKey);
	glutTimerFunc(MoveSpeed, timer, 1);
	glutReshapeFunc(Resize);
	glutMainLoop();
}