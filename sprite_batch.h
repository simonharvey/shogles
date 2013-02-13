//
//  sprite_batch.h
//  Frontline
//
//  Created by Simon Harvey on 12-11-20.
//  Copyright (c) 2012 Simon Harvey. All rights reserved.
//

#ifndef Frontline_sprite_batch_h
#define Frontline_sprite_batch_h

#include <OpenGLES/ES2/gl.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"

using namespace shogles;
using namespace std;

struct Sprite
{
	uint sprite_id;
	glm::vec2 position;
	glm::vec2 size;
};

struct uv_quad_t
{
	glm::vec2 a, b, c, d;
};

class Atlas
{
	vector<uv_quad_t> uvs_;
	map<string, int> indices;

public:
	void register_sprite(const string &name, uv_quad_t quad)
	{
		uint idx = uvs_.size();
		indices[name] = idx;
		uvs_.push_back(quad);
	}
	
	const vector<uv_quad_t> &uvs() const {
		return uvs_;
	}
};

class SpriteBatch
{
public:
	
	struct vertex_data_t {
		GLfloat pos[2];
		glm::vec2 uv;
	};
	
private:
	
	vector<Sprite> sprites;
	vector<vertex_data_t> vertex_data;
	const Atlas &atlas;
	GLuint sprites_vbo;
	
public:
	
	SpriteBatch(const Atlas &atlas) : atlas(atlas)
	{
		glGenBuffers(1, &sprites_vbo);
	}
	
	~SpriteBatch()
	{
		glDeleteBuffers(1, &sprites_vbo);
	}
	
	void render()
	{
		Shader &shader = shogles::ShaderLoader::instance().get_shader("sprite-batch.shader");
		auto uvs = atlas.uvs();
		glUniform2fv(shader.uniform_loc("u_UV"), uvs.size()*4, (GLfloat *)&uvs);
		
		const uint N = sprites.size();
		vertex_data.reserve(N*4);
		
		for (int i=0; i<N; ++i) {
			const Sprite &sp = sprites[i];
			float x0 = sp.position.x - sp.size.x/2.0f, x1 = sp.position.x + sp.size.x/2.0f;
			float y0 = sp.position.y - sp.size.y/2.0f, y1 = sp.position.y + sp.size.y/2.0f;
			const uv_quad_t &uv = atlas.uvs()[sp.sprite_id];
			
			vertex_data[i*4]	= (vertex_data_t) {{x0, y0}, uv.a};
			vertex_data[i*4+1]	= (vertex_data_t) {{x0, y1}, uv.b};
			vertex_data[i*4+2]	= (vertex_data_t) {{x1, y1}, uv.c};
			vertex_data[i*4+3]	= (vertex_data_t) {{x1, y0}, uv.d};
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, sprites_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data[0], GL_DYNAMIC_DRAW);
	}
};

#endif
