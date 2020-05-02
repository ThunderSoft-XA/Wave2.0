#include "wave.h"




#include <stdlib.h>


#include "linmath.h"






//#define USE_DUMMY_VERTICES
#ifdef USE_DUMMY_VERTICES

#include "dummy.h"

little::DummyMesh g_mesh;

#else  //!USE_DUMMY_VERTICES

#include "mesh.h"

#define GRID_WIDTH	160
#define GRID_HEIGHT	GRID_WIDTH

#define WAVE_VERSION 2

#if WAVE_VERSION == 1
little::AnimatedMesh  g_mesh(GRID_WIDTH, GRID_HEIGHT);
#else // Not 1
little::FilteringMesh g_mesh(GRID_WIDTH, GRID_HEIGHT);
#endif //WAVE_VERSION

#endif //USE_DUMMY_VERTICES

little::AnimatedMesh &mesh = g_mesh;



//{{ Vertex Shaders
const GLchar *vs_mesh = STRINGIZE(
	uniform mat4 MVP;

	out vec4 FragColor;

#if 0
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec3 Color;

	void main(void)
	{
		FragColor = vec4(Color, 1.0);
		gl_Position = MVP * vec4(Position, 1.0);
	}
#else
	layout (location = 0) in vec4 Position;
	layout (location = 1) in vec4 Color;

	void main(void)
	{
		FragColor = vec4(Color);
		gl_Position = MVP * vec4(Position);
	}
#endif
);
//}} Vertex Shaders

//{{ Fragment Shaders
const GLchar *fs_mesh = STRINGIZE(
	in vec4 FragColor;

	layout (location = 0) out vec4 Color;

	void main(void)
	{
		Color = FragColor;
	}
);
//}} Fragment Shaders



using namespace little;



Transformation transformation;

GLuint Program;
GLint  MVP;
unsigned int ViewWidth, ViewHeight;


void WaveInitialize(unsigned int view_width, unsigned int view_height)
{
	ViewWidth  = view_width;
	ViewHeight = view_height;

	GL_DUMP(ViewWidth, ViewHeight);

	//glEnable(GL_DEBUG_OUTPUT);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, ViewWidth, ViewHeight);
	//glPointSize(1.0);
	// The default front face is GL_CCW
	//glFrontFace(GL_CW); //GL_CCW); //

	Program = CreateProgram(vs_mesh, 0, fs_mesh);
	glUseProgram(Program);
	MVP = glGetUniformLocation(Program, "MVP");
	PROGRAM_LOG(Program, "glGetUniformLocation");

    vec3 eye = { 1.5f, 0.0f, 1.5f};

    transformation.LookAt(eye);
    transformation.Perspective(90.0f, (float) view_width / view_height, 0.1f, 90.0f);
    transformation.EnableRotations(true, true, true);
	transformation.Zoom = 0.0f;
    transformation.Delta = 0.3f; //1.0f; //0.1f; //

	mesh.EnableResetData(true);
    mesh.SetMaxFrameNumber(50000);

    mesh.PressureThreshold   = view_width * 0.1;
    mesh.PressurePeriod      = view_width / 50;
    mesh.PressureCoefficient = 0.04;
    mesh.MaxDeltaTime        = 0.02;
    mesh.TimeStepMultiple    = 10;

	mesh.Initialize();

	PRINT_SEPARATOR();
	LOG_INFO("Program:", Program);
	LOG_INFO("ModelViewProjectionLocation:", MVP);
	mesh.Dump();
	//mesh.DumpIndexes();
	PRINT_SEPARATOR();

	GL_CHECK_ERRORS();
}



GLenum mode	= GL_TRIANGLE_STRIP;//GL_POINTS;//GL_LINE_STRIP_ADJACENCY;//GL_LINES;//GL_LINE_STRIP;//
GLenum type =
#ifdef GL_ES
GL_UNSIGNED_SHORT;
#else  //!GL_ES
GL_UNSIGNED_INT;
#endif //GL_ES



void WaveRender()
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glViewport(0, 0, ViewWidth, ViewHeight);

    mat4x4 mvp;
    transformation.Matrix(mvp);
    //M4x4_DUMP(mvp);

	glUseProgram(Program);
    glUniformMatrix4fv(MVP, 1, GL_FALSE, (const GLfloat *)mvp);

    mesh.RefreshBuffer();

    GLenum modes[] = { GL_POINTS, GL_TRIANGLE_STRIP };
    auto updater = [&](size_t cycle)
	{
    	int index = cycle % (sizeof(modes) / sizeof(modes[0]));

    	FOOTPRINT();
    	LOG_INFO("Cycle:", cycle);
    	LOG_INFO("Index:", index);

    	mode = modes[index];
    	if (0 == cycle % 2)
    	{
    		mesh.Colorize(cycle / 2 % 2 == 0);
    	}
	};

   	transformation.Update(updater);
   	mesh.Draw(mode, type);
	mesh.Update();

    GL_CHECK_ERRORS_ONESHOT();
}
