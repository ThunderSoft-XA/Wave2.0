/*
 * gl_aux.h
 *
 *  Created on: Nov 21, 2019
 *      Author: little
 */

#ifndef INCLUDE_GL_AUX_H_
#define INCLUDE_GL_AUX_H_





#include <iostream>
#include <cmath>
#include <functional>
#include <sys/time.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include "linmath.h"


#ifndef NDEBUG

#define __DEBUG_PRINT(stream, tag, m, v) \
	do { \
		stream << "[" << tag << "][" << __FILE__ << "@" << __LINE__ << "] " << (m) << " " << (v) << std::endl; \
	} while(0)

#define LOG_INFO(m, v)	__DEBUG_PRINT(std::cout, "INFO", m, v)
#define LOG_ERROR(m, v)	__DEBUG_PRINT(std::cerr, "ERROR", m, v)

#define PRINT_SEPARATOR() LOG_INFO("----------------------------------------------------", "")

#else //!NDEBUG

#define LOG_INFO(m, v)
#define LOG_ERROR(m, v)

#define PRINT_SEPARATOR()

#endif //NDEBUG



#ifndef NDEBUG

#define __GL_LOG(get_iv, get_log, obj, m) \
	do { \
		GLsizei len; \
		get_iv(obj, GL_INFO_LOG_LENGTH, &len); \
		/*LOG_INFO("Length:", len);*/ \
		if (len > 0) \
		{ \
			GLchar *log = new GLchar[len + 1]; \
			get_log(obj, len, &len, log); \
			/*std::cerr << "[ERROR][" << __FILE__ << "@" << __LINE__ << "] " << m << ": " << log << std::endl; */ \
			LOG_ERROR(m, log); \
			delete [] log; \
		} \
	} while(0)

#define SHADER_LOG(_shader, m)		__GL_LOG(glGetShaderiv, glGetShaderInfoLog, _shader, m)

#define PROGRAM_LOG(_program, m)	__GL_LOG(glGetProgramiv, glGetProgramInfoLog, _program, m)

#define GL_CHECK_ERRORS() \
	do { \
		GLenum e; \
		unsigned int count = 0; \
		unsigned int finite = 255; \
		while((e = glGetError()) != GL_NO_ERROR && finite--) \
		{ \
			count++; \
			LOG_ERROR("[GL ERROR]", e); \
		} \
		/*LOG_INFO("Count:", count);*/ \
	} while(0)

#define GL_CHECK_ERRORS_ONESHOT() \
	do { \
		static int toggle = 1; \
		if (toggle) \
		{ \
			toggle = 0; \
			GL_CHECK_ERRORS(); \
		} \
	} while(0)

#define GL_CHECK_SHADER_CONDITION(_shader, pname, error_condition) \
	do { \
		GLint params; \
		glGetShaderiv(_shader, pname, &params); \
		if (params == error_condition) SHADER_LOG(_shader, ""); \
	} while(0)

#define GL_CHECK_PROGRAM_CONDITION(_program, pname, error_condition) \
	do { \
		GLint params; \
		glGetProgramiv(_program, pname, &params); \
		if (params == error_condition) PROGRAM_LOG(_program, ""); \
	} while(0)

#else //!NDEBUG

#define SHADER_LOG(_shader, m)
#define PROGRAM_LOG(_program, m)
#define GL_CHECK_ERRORS()
#define GL_CHECK_ERRORS_ONESHOT()
#define GL_CHECK_SHADER_CONDITION(_shader, pname, error_condition, m)
#define GL_CHECK_PROGRAM_CONDITION(_program, pname, error_condition)

#endif //_DEBUG


#define GL_CHECK_CONDITION(condition, msg) \
	do { \
		if (!(condition)) { \
			LOG_ERROR("[GL ERROR]", msg); \
		} \
	} while(0)


#ifdef NDEBUG
#define FOOTPRINT()
#else //!NDEBUG
#define FOOTPRINT()	std::cout << "[INFO][" << __FILE__ << "@" << __LINE__ << "][" << __func__ << "]" << std::endl;
#endif //NDEBUG

#ifndef SAFE_DEL_ARRAY
#define SAFE_DEL_ARRAY(a) if (0 != (a)) { delete[] (a); (a) = 0; }
#endif //SAFE_DEL_ARRAY


#ifndef GL_ES
#define GL_ES
#endif


#ifdef GL_ES
#define STRINGIZE(shader)	"#version 300 es\n" "#pragma debug(on)\n" "precision mediump float;\n" #shader
//#define STRINGIZE(shader)	"#version 300 es\n" #shader
#else
#define STRINGIZE(shader)	"#version 330 core\n" #shader
#endif


namespace gl_aux
{


typedef struct _Vector3
{
	union {	GLfloat x; GLfloat r; };
	union {	GLfloat y; GLfloat g; };
	union {	GLfloat z; GLfloat b; };
} Vector3;

typedef struct _Vertex
{
	Vector3 position;
	Vector3 color;

	void Dump() const
	{
#ifndef NDEBUG
		std::cout << "X:" << this->position.x << ", "
				  << "Y:" << this->position.y << ", "
				  << "Z:" << this->position.z << ", "
				  << "R:" << this->color.r << ", "
				  << "G:" << this->color.g << ", "
				  << "B:" << this->color.b << std::endl;
#endif  //NDEBUG
	}
} Vertex;


template <class T>
class Matrix
{
public:
	Matrix(unsigned int num_row, unsigned int num_col)	:
		num_of_row(num_row),
		num_of_col(num_col),
		data(0)
	{
		FOOTPRINT();

		if(this->num_of_row > 0 && this->num_of_col > 0)
			data = new T[this->num_of_row * this->num_of_col]();
	}

	~Matrix()
	{
		FOOTPRINT();
		SAFE_DEL_ARRAY(data);
	}

public:
	// Indexing (parenthesis operator)
	const T& operator () (unsigned int nr, unsigned int nc) const
	{
		return data[ nr * num_of_col + nc ];
	}

	T& operator () (unsigned int nr, unsigned int nc)
	{
		return data[ nr * num_of_col + nc ];
	}

public:
	// Get dimensions
	unsigned int NumOfRow() const { return num_of_row; }
	unsigned int NumOfCol() const { return num_of_col; }

	void Reset(void)
	{
		for (unsigned int i = 0; i < this->num_of_row * this->num_of_col; i++)
			this->data[i] = 0;
	}

private:
	unsigned int num_of_row;
	unsigned int num_of_col;

	T* data;

private:
	// To prevent unwanted copying:
	Matrix(const Matrix<T> & m);
	Matrix& operator = (const Matrix<T> & m);
};



template <class T>
class Twins
{
public:
	Twins() :
		value(0),
		original(0)
	{

	}

public:
	T operator = (T v)
	{
		this->value = this->original = v;

		//INFO("Value:" << this->value << ", Original:" << this->original);

		return this->value;
	}

	operator T()
	{
		return this->value;
	}

	T operator += (T v)
	{
		this->value += v;

		return this->value;
	}

	T operator -= (T v)
	{
		this->value -= v;

		return this->value;
	}

public:
	void Reset(void)
	{
		this->value = this->original;

		//INFO("Value:" << this->value << ", Original:" << this->original);
		//INFO("");
	}

protected:
	T value;
	T original;
};



auto radian = [](double degree)
{
	return degree * M_PI / 180.0;
};


uint64_t GetRawTime();

// In microsecond
double GetTime();



GLuint CreateProgram(const GLchar * vs_src, const GLchar * gs_src, const GLchar * fs_src);



struct Transformation
{
	Transformation() :
		Zoom(0.0f),
		Delta(1.0f),
		threshold(360),
		adjusted_threhold(0),
		toggle(1),
		selector(0),
		cycle(0),
		angles{&gamma, &beta, &alpha},
		coefs{1, -1, 1},
		status{true, true, true}
	{
		mat4x4_identity(this->view);
		mat4x4_identity(this->projection);
	}

	void LookAt(vec3 eye, vec3 center = 0, vec3 up = 0);

	void Perspective(float y_fov, float aspect, float n = 0.1f, float f = 20.0f)
	{
		mat4x4_perspective(this->projection, radian(y_fov), aspect, n, f);
	}

	void EnableRotations(bool alpha = true, bool beta = true, bool gamma = true)
	{
		status[2] = alpha;
		status[1] = beta;
		status[0] = gamma;
	}

	// Gets MVP matrix
	void Matrix(mat4x4 m);

	void Update(std::function<void (size_t)> callback);

	size_t GetCycle() const { return this->cycle; }

	GLfloat Zoom;
	GLfloat Delta;

protected:
	mat4x4 view;
	mat4x4 projection;

	Twins<GLfloat> alpha;
	Twins<GLfloat> beta;
	Twins<GLfloat> gamma;

	GLfloat threshold;
	GLfloat adjusted_threhold;

	int toggle;
	int selector;

	size_t cycle;

	static const size_t NUM_ANGLE = 3;

	Twins<GLfloat> * angles[NUM_ANGLE];
	int	             coefs[NUM_ANGLE];
	bool             status[NUM_ANGLE];
};



} //gl_aux



#ifndef NDEBUG

#define GL_DUMP(view_width, view_height) \
	do { \
		int major = 0; \
		int minor = 0; \
		int context_profile = 0; \
		int n = 0; \
		\
		glGetIntegerv(GL_MAJOR_VERSION, &major); \
		glGetIntegerv(GL_MINOR_VERSION, &minor); \
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &context_profile); \
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n); \
		\
		PRINT_SEPARATOR(); \
		LOG_INFO("Width:", view_width); \
		LOG_INFO("Height:", view_height); \
		LOG_INFO("OpenGLMajorVersion:", major); \
		LOG_INFO("OpenGLMinorVersion:", minor); \
		LOG_INFO("OpenGLVersion:", glGetString(GL_VERSION)); \
		LOG_INFO("OpenGLVendor:", glGetString(GL_VENDOR)); \
		LOG_INFO("OpenGLRendererName:", glGetString(GL_RENDERER)); \
		LOG_INFO("OpenGLContextProfile:", context_profile); \
		LOG_INFO("OpenGLShadingLanguageVersion:", glGetString(GL_SHADING_LANGUAGE_VERSION)); \
		LOG_INFO("GL_MAX_VERTEX_ATTRIBS:", GL_MAX_VERTEX_ATTRIBS); \
		LOG_INFO("MaxVertexAttribs:", n); \
		PRINT_SEPARATOR(); \
	} while(0)

#define M4x4_DUMP(m) \
	do { \
		PRINT_SEPARATOR(); \
		LOG_INFO("Matrix:", #m); \
		for (int i = 0; i < 4; i++) \
		{ \
			for (int j = 0; j < 4; j++) \
				std::cout << m[i][j] << " "; \
			std::cout << std::endl; \
		} \
		PRINT_SEPARATOR(); \
	} while(0)

#else  //!NDEBUG

#define GL_DUMP(view_width, view_height)
#define M4x4_DUMP(m)

#endif //NDEBUG



#endif /* INCLUDE_GL_AUX_H_ */
