/*
 * gl_aux.cpp
 *
 *  Created on: Nov 21, 2019
 *      Author: little
 */

#include "gl_aux.h"







namespace gl_aux
{



uint64_t GetRawTime()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (uint64_t) tv.tv_sec * (uint64_t)1000000 + (uint64_t)tv.tv_usec;
}



// In microsecond
double GetTime()
{
	static uint64_t	base_time = 0;

	if ( 0 == base_time )
	{
		base_time = GetRawTime();
	}

	return (double)(GetRawTime() - base_time) * 1e-6; // Microsecond
}



GLuint CreateProgram(const GLchar * vs_src, const GLchar * gs_src, const GLchar * fs_src)
{
	//GL_CHECK_CONDITION(vs_src != 0, "Vertex shader is missing!","");

    GLuint program = glCreateProgram();

    auto attach_shader = [](GLuint program, GLenum shader_type, const GLchar **shader_source)
		{
    		GLuint shader = glCreateShader(shader_type);

    		glShaderSource(shader, 1, shader_source, 0);
    		glCompileShader(shader);
    		//GL_CHECK_SHADER_CONDITION(shader, GL_COMPILE_STATUS, GL_FALSE);
    		glAttachShader(program, shader);

    		return shader;
		};

    attach_shader(program, GL_VERTEX_SHADER, &vs_src);
    //if (gs_src)	attach_shader(program, GL_GEOMETRY_SHADER, &gs_src);
    if (fs_src)	attach_shader(program, GL_FRAGMENT_SHADER, &fs_src);

    glLinkProgram(program);

    GL_CHECK_PROGRAM_CONDITION(program, GL_LINK_STATUS, GL_FALSE);

    return program;
}



void Transformation::LookAt(vec3 eye, vec3 center, vec3 up)
{
	if (!eye)
	{
		static vec3 _eye = {1.0f, 1.0f, 1.0f};
		eye = _eye;
	};
	if (!center)
	{
		static vec3 _center = {0.0f, 0.0f, 0.0f};
		center = _center;
	};
	if (!up)
	{
		static vec3 _up = {0.0f, 0.0f, 1.0f};
		up = _up;
	};

	mat4x4_look_at(this->view, eye, center, up);
}



void Transformation::Matrix(mat4x4 m)
{
	mat4x4 model;

	mat4x4_identity(model);
	mat4x4_translate(model, 0.0f, 0.0f, this->Zoom);
	mat4x4_rotate_X(model, model, radian(this->alpha));
	mat4x4_rotate_Y(model, model, radian(this->beta));
	mat4x4_rotate_Z(model, model, radian(this->gamma));

	//Projection * View * Model
	mat4x4_mul(m, this->projection, this->view);
	mat4x4_mul(m, m, model);
}



void Transformation::Update(std::function<void (size_t)> callback)
{
	if (callback && this->selector % NUM_ANGLE == 0 && 1 == this->toggle)
	{
		this->cycle++;

		callback(this->cycle);
	}

	auto inc_selector = [&]()
		{
			this->selector = (this->selector + 1) % NUM_ANGLE;
		};

	// Disable rotation
	if (!status[this->selector])
	{
		inc_selector();
		this->toggle = 1;

		LOG_INFO("Selector:", this->selector);

		return;
	}

	Twins<GLfloat> & angle = *angles[selector];

	if (1 == this->toggle)
	{
		angle.Reset();
		this->adjusted_threhold = angle + coefs[selector] * this->threshold;
		this->toggle = 0;

		LOG_INFO("Selector:", this->selector);
		LOG_INFO("Threshold:", this->threshold);
	}

	angle += coefs[selector] * this->Delta;

	if (std::abs(angle) >= std::abs(this->adjusted_threhold))
	{
		inc_selector();
		this->toggle = 1;

		LOG_INFO("Selector:", this->selector);
	}
}



} //namespace gl_aux

