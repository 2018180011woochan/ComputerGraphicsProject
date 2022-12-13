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

#include "FixMonster.h"

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
void MySpecialKey(int Key, int X, int Y);

//func
void timer(int value);
void ObjList();
void drawscene();


int WIDTH = 0;
int HEIGHT = 0;

enum PLAYERDIR { UP, DOWN, LEFT, RIGHT, END };
enum BULLETDIR { BULLET_UP, BULLET_DOWN, BULLET_LEFT, BULLET_RIGHT };
enum WEAPON { SWORD, GUN };
PLAYERDIR _dir = PLAYERDIR::UP;
WEAPON _weapon = WEAPON::GUN;

AABB _PlayerAABB;

bool CrashCheck(AABB pAABB1, AABB pAABB2);

//bullet
struct Bullet {
    float x;
    float y;
    float z;
    BULLETDIR _bulletDir = BULLETDIR::BULLET_UP;
    AABB _bulletAABB;
};
Bullet BulletLocation[20];
int BulletIndex = 0;

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

    float SwordAttackAngle = 0.0f;
    float GunAngle = 0.0f;
    float SwordAngle = 0.0f;

    float anglecamera = 160.0f;
    float anglecamera2 = 0.0f;

    float LightRadian = 10.0f;

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

bool isAttack = false;
bool isDamaged = false;
bool JumpCheck = false;

glm::vec3 objC = glm::vec3(0, 0, 0);
glm::vec3 cameraPos = glm::vec3(1.0f, 3.0f, 10.0f);
glm::vec3 lightPos = glm::vec3(0, 3.0f, 2.5f);
glm::vec3 lightColor = glm::vec3(1.4f, 1.3f, 1.3f);
glm::vec3 Cameraposdir = glm::vec3(0.0f);
glm::vec3 Cameradir = glm::vec3(0.0f);
float Movevalue = 0.8f;

CGameObject* _monsters[10];

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
    glutSpecialFunc(MySpecialKey);
    glutTimerFunc(1, timer, 1);
    glutMainLoop();
}

void timer(int value)
{
    //cout << "x : " << TransList.T_Bodyx << ", z : " << TransList.T_Bodyz << endl;

    if (_dir == PLAYERDIR::UP)
        AngleList.angley = 0.f;
    if (_dir == PLAYERDIR::DOWN)
        AngleList.angley = 180.f;
    if (_dir == PLAYERDIR::RIGHT)
        AngleList.angley = 90.f;
    if (_dir == PLAYERDIR::LEFT)
        AngleList.angley = -90.f;

    if (_weapon == WEAPON::SWORD) {
        if (isAttack) {
            AngleList.SwordAttackAngle += 30.f;

            AABB SwordRange;
            SwordRange.maxX = TransList.T_Bodyx + 2.f;
            SwordRange.minX = TransList.T_Bodyx - 2.f;
            SwordRange.maxZ = TransList.T_Bodyz + 2.f;
            SwordRange.minZ = TransList.T_Bodyz - 2.f;

            for (int i = 0; i < 10; ++i) {
                if (!CrashCheck(SwordRange, _monsters[i]->_AABB)) 
                    _monsters[i]->HP -= 2.f;
                
            }
        }
        if (AngleList.SwordAttackAngle > 150) {
            isAttack = false;
            AngleList.SwordAttackAngle = 0.f;
        }
    }

    for (int i = 0; i < 20; ++i) {
        switch (BulletLocation[i]._bulletDir) {
        case BULLETDIR::BULLET_UP:
            BulletLocation[i].z += 0.5f;
            break;
        case BULLETDIR::BULLET_DOWN:
            BulletLocation[i].z -= 0.5f;
            break;
        case BULLETDIR::BULLET_LEFT:
            BulletLocation[i].x += 0.5f;
            break;
        case BULLETDIR::BULLET_RIGHT:
            BulletLocation[i].x -= 0.5f;
            break;
        }
    }

    for (int i = 0; i < 20; ++i) {
        BulletLocation[i]._bulletAABB.maxX = BulletLocation[i].x + 0.5f;
        BulletLocation[i]._bulletAABB.minX = BulletLocation[i].x - 0.5f;
        BulletLocation[i]._bulletAABB.maxZ = BulletLocation[i].z + 0.5f;
        BulletLocation[i]._bulletAABB.minZ = BulletLocation[i].z - 0.5f;

        if (BulletLocation[i].x == 99.f)
            continue;

        for (int j = 0; j < 10; ++j) {
            if (!CrashCheck(BulletLocation[i]._bulletAABB, _monsters[j]->_AABB)) 
                _monsters[j]->HP -= 2.f;           
        }
    }

    for (int i = 0; i < 10; ++i) {
        static_cast<FixMonster*>(_monsters[i])->Update(TransList.T_Bodyx, TransList.T_Bodyz);
    }

    for (int i = 0; i < 10; ++i) {
        if (!CrashCheck(_PlayerAABB, _monsters[i]->_AABB)) {
            if (!isDamaged)
                isDamaged = true;
        }
    }

    if (isDamaged) {
        if (TransList.T_Bodyy < 0) {
            JumpCheck = false;
            isDamaged = false;
            TransList.T_Bodyy = 0.0f;
        }

        if (TransList.T_Bodyy >= 2.5f)
            JumpCheck = true;
        
        if (!JumpCheck) {
            TransList.T_Bodyy += 0.1f;
            switch (_dir)
            {
            case PLAYERDIR::LEFT:
                TransList.T_Bodyx -= 0.1f;
                break;
            case PLAYERDIR::RIGHT:
                TransList.T_Bodyx += 0.1f;
                break;
            case PLAYERDIR::UP:
                TransList.T_Bodyz -= 0.1f;
                break;
            case PLAYERDIR::DOWN:
                TransList.T_Bodyz += 0.1f;
                break;
            case END:
                break;
            default:
                break;
            }
        }
        else
            TransList.T_Bodyy -= 0.1f;
    }

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

void MySpecialKey(int Key, int X, int Y)
{
    switch (Key) {
    case GLUT_KEY_LEFT:     //왼쪽 키
        _dir = PLAYERDIR::LEFT;
        TransList.T_Bodyx += Movevalue;
        AngleList.GunAngle = 90.f;
        AngleList.SwordAngle = 90.f;
        break;
    case GLUT_KEY_RIGHT:     //오른쪽 키
        _dir = PLAYERDIR::RIGHT;
        TransList.T_Bodyx -= Movevalue;
        AngleList.GunAngle = 270.f;
        AngleList.SwordAngle = 270.f;
        break;
    case GLUT_KEY_UP:      //위 키
        _dir = PLAYERDIR::UP;
        TransList.T_Bodyz += Movevalue;
        AngleList.GunAngle = 0.0f;
        AngleList.SwordAngle = 0.0f;
        break;
    case GLUT_KEY_DOWN:      //아래 키
        _dir = PLAYERDIR::DOWN;
        TransList.T_Bodyz -= Movevalue;
        AngleList.GunAngle = 180.f;
        AngleList.SwordAngle = 180.f;
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        _weapon = WEAPON::GUN;
        break;
    case '2':
        _weapon = WEAPON::SWORD;
        break;
    case 'a':
        isAttack = true;

        if (BulletIndex >= 19)
            BulletIndex = 0;

        if (_weapon == WEAPON::GUN) {
            BulletIndex += 1;
            BulletLocation[BulletIndex].x = TransList.T_Bodyx;
            BulletLocation[BulletIndex].y = TransList.T_Bodyy + 2.0f;
            BulletLocation[BulletIndex].z = TransList.T_Bodyz;


            switch (_dir) {
            case PLAYERDIR::UP:
                BulletLocation[BulletIndex]._bulletDir = BULLETDIR::BULLET_UP;
                break;
            case PLAYERDIR::DOWN:
                BulletLocation[BulletIndex]._bulletDir = BULLETDIR::BULLET_DOWN;
                break;
            case PLAYERDIR::LEFT:
                BulletLocation[BulletIndex]._bulletDir = BULLETDIR::BULLET_LEFT;
                break;
            case PLAYERDIR::RIGHT:
                BulletLocation[BulletIndex]._bulletDir = BULLETDIR::BULLET_RIGHT;
                break;
            }
        }
        break;
    case'q':
        for (int i = 0; i < 10; ++i) {
            delete _monsters[i];
            _monsters[i] = nullptr;
        }
        glutLeaveMainLoop();
        break;
    }
    glutPostRedisplay();
}

void InitBuffer()
{
    glGenVertexArrays(26, VAO);

    for (int i = 0; i <= 6; i++)
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


    for (int i = 0; i < 20; ++i) {
        BulletLocation[i].x = 99.f;
        BulletLocation[i].y = 99.f;
        BulletLocation[i].z = 99.f;
    }

    for (int i = 0; i < 10; ++i) {
        _monsters[i] = new FixMonster(i);
        _monsters[i]->_yPos = 2.f;
        _monsters[i]->objSpeed = 0.05f;
        _monsters[i]->HP = 50.f;
    }

#pragma region 수동위치정해주기 ㄱ-
    _monsters[0]->_xPos = 6.4f;
    _monsters[0]->_zPos = 18.8f;

    _monsters[1]->_xPos = -2.4f;
    _monsters[1]->_zPos = 38.f;

    _monsters[2]->_xPos = 1.6f;
    _monsters[2]->_zPos = 64.4f;

    _monsters[3]->_xPos = -7.2f;
    _monsters[3]->_zPos = 15.6f;

    _monsters[4]->_xPos = 40.8f;
    _monsters[4]->_zPos = 44.4f;

    _monsters[5]->_xPos = 41.6f;
    _monsters[5]->_zPos = 17.2f;

    _monsters[6]->_xPos = 68.f;
    _monsters[6]->_zPos = 17.2f;

    _monsters[7]->_xPos = 116.f;
    _monsters[7]->_zPos = 60.4f;

    _monsters[8]->_xPos = 116.f;
    _monsters[8]->_zPos = 112.4f;

    _monsters[9]->_xPos = 97.f;
    _monsters[9]->_zPos = 120.f;
#pragma endregion
}

void ObjList()
{
    readTriangleObj("OBJ/plane3.obj", Vertex[0], Texture[0], Nomal[0]);
    readTriangleObj("OBJ/cube.obj", Vertex[1], Texture[1], Nomal[1]);
    readTriangleObj("OBJ/cube.obj", Vertex[2], Texture[2], Nomal[2]);
    readTriangleObj("OBJ/Sphere.obj", Vertex[3], Texture[3], Nomal[3]);
    readTriangleObj("OBJ/gun.obj", Vertex[4], Texture[4], Nomal[4]);
    readTriangleObj("OBJ/sword.obj", Vertex[5], Texture[5], Nomal[5]);
    readTriangleObj("OBJ/crown.obj", Vertex[6], Texture[6], Nomal[6]);

}

bool CrashCheck(AABB a, AABB b)
{
    a.lengthX = abs(a.maxX - a.minX);
    a.lengthZ = abs(a.maxZ - a.minZ);

    b.lengthX = abs(b.maxX - b.minX);
    b.lengthZ = abs(b.maxZ - b.minZ);

    if ((b.minX + b.lengthX > a.minX + a.lengthX) && (a.minX + a.lengthX > b.minX) &&
        (b.minZ + b.lengthZ > a.minZ) && (a.minZ + a.lengthZ > b.minZ))
        return false;
    return true;
}

void drawscene()
{
    glUseProgram(shaderID);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            glBindVertexArray(VAO[0]);   // 바닥
            unsigned int StageBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
            glUniform1i(StageBlendCheck, 2);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
            glm::mat4 StageTrasMatrix = glm::mat4(1.0f);
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(-50.f + 10.f * i, 0.f, 0.f + 10.f * j));
            //StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(70.f), glm::vec3(0.0f, 1.0f, 0.0f));
            StageTrasMatrix = glm::scale(StageTrasMatrix, glm::vec3(10.0, 1.0, 10.0));
            unsigned int StageTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
            glUniformMatrix4fv(StageTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageTrasMatrix));
            glm::mat4 StageNormalMatrix = glm::mat4(1.0f);
            unsigned int StageNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
            glUniformMatrix4fv(StageNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageNormalMatrix));
            glDrawArrays(GL_TRIANGLES, 0, Vertex[0].size());
        }
    }

#pragma region 미로
    for (int i = 0; i < 20; ++i)
    {
        glBindVertexArray(VAO[1]);   // 미로
        unsigned int mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        glm::mat4 mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(140.f, 10.f, 5.f + i * 10.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        unsigned int mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        glm::mat4 mazeNormalMatrix = glm::mat4(1.0f);
        unsigned int mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(-10.f + i * 10.f, 10.f, 180.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

        if (i > 11)
            continue;
        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(-20.f, 10.f, 5.f + i * 10.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(-10.f + i * 10.f, 10.f, 100.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(30.f + i * 10.f, 10.f, 0.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());



        if (i > 5)
            continue;
        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(20.f, 10.f, 5.f + i * 10.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());

        glBindVertexArray(VAO[1]);   // 미로
        mazeBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(mazeBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
        mazeTrasMatrix = glm::mat4(1.0f);
        mazeTrasMatrix = glm::translate(mazeTrasMatrix, glm::vec3(60.f, 10.f, 40.f + i * 10.f));
        mazeTrasMatrix = glm::scale(mazeTrasMatrix, glm::vec3(10.0, 20.0, 10.0));
        mazeTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(mazeTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeTrasMatrix));
        mazeNormalMatrix = glm::mat4(1.0f);
        mazeNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(mazeNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(mazeNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());
    }
#pragma endregion 미로

#pragma region 플레이어
    glBindVertexArray(VAO[1]);   // 플레이어 몸통
    unsigned int playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(playerBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    glm::mat4 playerTrasMatrix = glm::mat4(1.0f);
    playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
    playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
    playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(1.0, 1.3, 1.0));

    _PlayerAABB.maxX = TransList.T_Bodyx + 1.0f;
    _PlayerAABB.minX = TransList.T_Bodyx - 1.0f;
    _PlayerAABB.maxZ = TransList.T_Bodyz + 1.0f;
    _PlayerAABB.minZ = TransList.T_Bodyz - 1.0f;

    unsigned int playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
    glm::mat4 playerNormalMatrix = glm::mat4(1.0f);
    unsigned int playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

    glBindVertexArray(VAO[2]);   // 플레이어 팔
    playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(playerBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    playerTrasMatrix = glm::mat4(1.0f);
    if (_dir == PLAYERDIR::UP || _dir == PLAYERDIR::DOWN)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
    if (_dir == PLAYERDIR::LEFT || _dir == PLAYERDIR::RIGHT)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz + 0.5f));
    playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
    playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.4f, 0.7f, 0.4f));
    playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
    playerNormalMatrix = glm::mat4(1.0f);
    playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

    glBindVertexArray(VAO[2]);   // 플레이어 팔
    playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(playerBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    playerTrasMatrix = glm::mat4(1.0f);
    if (_dir == PLAYERDIR::UP || _dir == PLAYERDIR::DOWN)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
    if (_dir == PLAYERDIR::LEFT || _dir == PLAYERDIR::RIGHT)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz - 0.5f));
    playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
    playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.4f, 0.7f, 0.4f));
    playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
    playerNormalMatrix = glm::mat4(1.0f);
    playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

    glBindVertexArray(VAO[2]);   // 플레이어 다리
    playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(playerBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    playerTrasMatrix = glm::mat4(1.0f);
    if (_dir == PLAYERDIR::UP || _dir == PLAYERDIR::DOWN)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.35f, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz));
    if (_dir == PLAYERDIR::LEFT || _dir == PLAYERDIR::RIGHT)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz - 0.35f));
    playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
    playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.3f, 0.8f, 0.3f));
    playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
    playerNormalMatrix = glm::mat4(1.0f);
    playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

    glBindVertexArray(VAO[2]);   // 플레이어 다리
    playerBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(playerBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    playerTrasMatrix = glm::mat4(1.0f);
    if (_dir == PLAYERDIR::UP || _dir == PLAYERDIR::DOWN)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.35f, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz));
    if (_dir == PLAYERDIR::LEFT || _dir == PLAYERDIR::RIGHT)
        playerTrasMatrix = glm::translate(playerTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 1.0f, TransList.T_Bodyz + 0.35f));
    playerTrasMatrix = glm::rotate(playerTrasMatrix, glm::radians(AngleList.angley), glm::vec3(0.0f, 1.0f, 0.0f));
    playerTrasMatrix = glm::scale(playerTrasMatrix, glm::vec3(0.3f, 0.8f, 0.3f));
    playerTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(playerTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerTrasMatrix));
    playerNormalMatrix = glm::mat4(1.0f);
    playerNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(playerNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(playerNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());

    glBindVertexArray(VAO[3]);   // 플레이어 머리
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

#pragma region 총
    glBindVertexArray(VAO[4]);   // 총
    unsigned int StageBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(StageBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    glm::mat4 StageTrasMatrix = glm::mat4(1.0f);
    if (_weapon == WEAPON::GUN)
    {
        if (_dir == PLAYERDIR::UP)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx + 1.f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(AngleList.GunAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::DOWN)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(AngleList.GunAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if (_dir == PLAYERDIR::LEFT)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz + 0.5f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(AngleList.GunAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::RIGHT)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz - 0.5f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(AngleList.GunAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
    else
    {
        if (_dir == PLAYERDIR::UP)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 1.f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::DOWN)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz + 0.2f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::LEFT)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.3f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 0.5f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::RIGHT)
        {
            StageTrasMatrix = glm::translate(StageTrasMatrix, glm::vec3(TransList.T_Bodyx + 1.0f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 0.5f));
            StageTrasMatrix = glm::rotate(StageTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
    StageTrasMatrix = glm::scale(StageTrasMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
    unsigned int StageTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(StageTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageTrasMatrix));
    glm::mat4 StageNormalMatrix = glm::mat4(1.0f);
    unsigned int StageNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(StageNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(StageNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[4].size());
#pragma endregion 총

#pragma region 칼
    glBindVertexArray(VAO[5]);   // 총
    unsigned int SwordBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
    glUniform1i(SwordBlendCheck, 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glUniform1i(glGetUniformLocation(shaderID, "textureC"), 0);
    glm::mat4 SwordTrasMatrix = glm::mat4(1.0f);
    if (_weapon == WEAPON::SWORD)
    {
        if (_dir == PLAYERDIR::UP)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAttackAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::DOWN)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx + 0.5f, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAttackAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        }

        if (_dir == PLAYERDIR::LEFT)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz + 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAttackAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::RIGHT)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx, TransList.T_Bodyy + 2.0f, TransList.T_Bodyz - 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(AngleList.SwordAttackAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
    else
    {
        if (_dir == PLAYERDIR::UP)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(135.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::DOWN)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.5f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz + 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(135.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::LEFT)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx - 0.6f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(135.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (_dir == PLAYERDIR::RIGHT)
        {
            SwordTrasMatrix = glm::translate(SwordTrasMatrix, glm::vec3(TransList.T_Bodyx + 1.0f, TransList.T_Bodyy + 2.5f, TransList.T_Bodyz - 0.5f));
            SwordTrasMatrix = glm::rotate(SwordTrasMatrix, glm::radians(135.f), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
    SwordTrasMatrix = glm::scale(SwordTrasMatrix, glm::vec3(15.f, 15.f, 15.f));
    unsigned int SwordTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(SwordTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(SwordTrasMatrix));
    glm::mat4 SwordNormalMatrix = glm::mat4(1.0f);
    unsigned int SwordNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
    glUniformMatrix4fv(SwordNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(SwordNormalMatrix));
    glDrawArrays(GL_TRIANGLES, 0, Vertex[5].size());
#pragma endregion 칼

#pragma region 총알
    for (int i = 0; i < 20; ++i) {
        glBindVertexArray(VAO[1]);
        unsigned int BulletBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(BulletBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glm::mat4 BulletTrasMatrix = glm::mat4(1.0f);
        BulletTrasMatrix = glm::translate(BulletTrasMatrix, glm::vec3(BulletLocation[i].x, BulletLocation[i].y, BulletLocation[i].z));
        BulletTrasMatrix = glm::scale(BulletTrasMatrix, glm::vec3(0.5, 0.5, 0.5));
        unsigned int BulletTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(BulletTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(BulletTrasMatrix));
        glm::mat4 BulletNormalMatrix = glm::mat4(1.0f);
        unsigned int BulletNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(BulletNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(BulletNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[2].size());
    }
#pragma endregion 총알

#pragma region fixMonster
    for (int i = 0; i < 10; ++i) {
        glBindVertexArray(VAO[1]);
        unsigned int BulletBlendCheck = glGetUniformLocation(shaderID, "Blendcheck");
        glUniform1i(BulletBlendCheck, 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[6]);
        glm::mat4 BulletTrasMatrix = glm::mat4(1.0f);
        //BulletTrasMatrix = glm::translate(BulletTrasMatrix, glm::vec3(0.f, 2.f, 20.f));
        BulletTrasMatrix = glm::translate(BulletTrasMatrix, glm::vec3(_monsters[i]->_xPos, _monsters[i]->_yPos, _monsters[i]->_zPos));
        _monsters[i]->_AABB.maxX = _monsters[i]->_xPos + 2.f;
        _monsters[i]->_AABB.minX = _monsters[i]->_xPos - 2.f;
        _monsters[i]->_AABB.maxZ = _monsters[i]->_zPos + 2.f;
        _monsters[i]->_AABB.minZ = _monsters[i]->_zPos - 2.f;

        BulletTrasMatrix = glm::scale(BulletTrasMatrix, glm::vec3(2, 2, 2));
        unsigned int BulletTransMatrixLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(BulletTransMatrixLocation, 1, GL_FALSE, glm::value_ptr(BulletTrasMatrix));
        glm::mat4 BulletNormalMatrix = glm::mat4(1.0f);
        unsigned int BulletNormalMatrixLocation = glGetUniformLocation(shaderID, "normalTransform");
        glUniformMatrix4fv(BulletNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(BulletNormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, Vertex[1].size());
    }
#pragma endregion fixMonster
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
    unsigned char* TailData = stbi_load("Texture/grass.jpg", &TailWidthImage, &TailHeightImage, &TailnumberOfChannel, 0);
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

    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int tempWidthImage, tempHeightImage, tempnumberOfChannel;
    unsigned char* tempData = stbi_load("Texture/dust.jpg", &tempWidthImage, &tempHeightImage, &tempnumberOfChannel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tempWidthImage, tempHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, tempData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tempData);

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int FHWidthImage, FHHeightImage, FHnumberOfChannel;
    unsigned char* FHData = stbi_load("Texture/FireHely.jpg", &FHWidthImage, &FHHeightImage, &FHnumberOfChannel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FHWidthImage, FHHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, FHData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(FHData);

    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int MonsterWidthImage, MonsterHeightImage, MonsternumberOfChannel;
    unsigned char* MonsterData = stbi_load("Texture/fmonster.jpg", &MonsterWidthImage, &MonsterHeightImage, &MonsternumberOfChannel, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MonsterWidthImage, MonsterHeightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, MonsterData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(MonsterData);

}