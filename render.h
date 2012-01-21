//
//  render.h
//  Mesh
//
//  Created by Simon Harvey on 12-01-15.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Mesh_render_h
#define Mesh_render_h

#include <GLKit/GLKit.h>
#include "shader.h"
#include "utils.h"

using namespace shogles;

void render_point(const GLKMatrix4 &proj, const GLKVector3 &pos, uint color=0xFF0000)
{
	static const Shader &shader = ShaderLoader::get_instance().get_shader(abs_path("Shaders/color.shader"));
	shader.use();
	
	int r = (color >> 16) & 0xff;
	int g = (color >> 8) & 0xff;
	int b = color & 0xff;
	glUniform4f(shader.uniform_loc("color"), (float)r/255, (float)g/255, (float)b/255, 1);
	
	glUniformMatrix4fv(shader.uniform_loc("projection"), 1, GL_FALSE, proj.m);
	glVertexAttribPointer(shader.attrib_loc("position"), 3, GL_FLOAT, GL_FALSE, 0, &pos.v);
	glEnableVertexAttribArray(shader.attrib_loc("position"));
	
	glDrawArrays(GL_POINTS, 0, 1);
}

void render_ray(const GLKMatrix4 &proj, const GLKVector3 &start, const GLKVector3 &direction, float length)
{
	static const Shader &shader = ShaderLoader::get_instance().get_shader(abs_path("Shaders/color.shader"));
	shader.use();
	glUniformMatrix4fv(shader.uniform_loc("projection"), 1, GL_FALSE, proj.m);
	float pts[2][3];
	memcpy(pts[0], start.v, sizeof(start.v));
	memcpy(pts[1], GLKVector3Add(start, GLKVector3MultiplyScalar(direction, length)).v, sizeof(start.v));
	//pts[0] = start.v;
	/*float vec[2] = { 
		start.v, 
		GLKVector3Add(start, GLKVector3MultiplyScalar(direction, length)).v 
	};*/
	glVertexAttribPointer(shader.attrib_loc("position"), 3, GL_FLOAT, GL_FALSE, 0, pts);
	glEnableVertexAttribArray(shader.attrib_loc("position"));
	
	glDrawArrays(GL_LINES, 0, 2);
}

#endif
