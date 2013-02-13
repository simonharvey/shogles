//
//  sprite.h
//  Frontline
//
//  Created by Simon Harvey on 12-12-29.
//  Copyright (c) 2012 Simon Harvey. All rights reserved.
//

#ifndef shogles_sprite_h
#define shogles_sprite_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>
#include <map>
#include "resource_manager.h"

using namespace std;

#ifndef BATCH_MAX_SPRITES
#define BATCH_MAX_SPRITES 1024
#endif

namespace shogles
{
	using namespace glm;
	
	struct Sprite
	{
		glm::vec2 position;
		glm::vec2 scale;
	};
	
	struct SpriteDef
	{
		glm::vec2 size;
		glm::vec2 uvs[4];
	};
	
	struct AtlasSprite : Sprite
	{
		SpriteDef *def;
	};
	
	struct Atlas
	{
		TexturePtr texture;
		map<string, SpriteDef> definitions;
		
		void register_sprite(const string &name, vec2 position, vec2 size)
		{
			auto texsize = texture->size();
			float u0 = position.x / texsize.x, v0 = position.y / texsize.y;
			float u1 = u0 + size.x / texsize.x, v1 = v0 + size.y / texsize.y;
			SpriteDef def {
				size, {
					vec2(u0, v0), vec2(u1, v0), vec2(u0, v1), vec2(u1, v1)
				}
			};
			definitions[name] = def;
		}
	};
	
	struct SpriteBatch
	{
		// we use a index buffer since no matter what, the indices we'll draw
		// are always in the same pattern. just reserve a bunch up front and be
		// done with it.
		GLuint index_buffer;
		
		Atlas &atlas;
		vector<AtlasSprite *> sprites;
		ShaderPtr shader;
		
		SpriteBatch(Atlas &atlas) : atlas(atlas)
		{
			shader = ResourceManager::get_instance().get_shader("Assets/sprite.shader");
		
			// gen the buffers
			glGenBuffers(1, &index_buffer);
			auto *indices = gen_quads_indices<GLushort>(BATCH_MAX_SPRITES);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2048, indices, GL_STATIC_DRAW);
			delete indices;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
		~SpriteBatch()
		{
			glDeleteBuffers(1, &index_buffer);
		}
			
		AtlasSprite &create_sprite(const string &name)
		{
			AtlasSprite *sp = new AtlasSprite();// { &atlas.definitions[name] };
			sp->def = &atlas.definitions[name];
			sp->scale = vec2(1, 1);
			sprites.push_back(sp);
			return *sp;
		}
		
		struct VertexData
		{
			GLfloat pos[2];
			GLfloat uv[2];
		};
		
		void render()
		{
			shader->use(); // forgot that stupid line!
			
			VertexData *verts = new VertexData[sprites.size() * 4];
			
			for (int i=0; i<sprites.size(); ++i)
			{
				AtlasSprite &sp = *sprites[i];
				vec2 size = sp.def->size;
				auto uvs = sp.def->uvs;
				float x0 = sp.position.x - sp.scale.x*size.x/2.0f, x1 = sp.position.x + sp.scale.x*size.x/2.0f;
				float y0 = sp.position.y - sp.scale.y*size.y/2.0f, y1 = sp.position.y + sp.scale.x*size.y/2.0f;
				verts[i*4]		= { {x0, y0}, {uvs[0].x, uvs[0].y} };
				verts[i*4+1]	= { {x1, y0}, {uvs[1].x, uvs[1].y} };
				verts[i*4+2]	= { {x0, y1}, {uvs[2].x, uvs[2].y} };
				verts[i*4+3]	= { {x1, y1}, {uvs[3].x, uvs[3].y} };
			}
			
			UNIFORM(shader, u_Projection);
			UNIFORM(shader, u_Tex);
			ATTRIB(shader, a_Pos);
			ATTRIB(shader, a_TexCoord);
			
			glEnableVertexAttribArray(a_Pos);
			glEnableVertexAttribArray(a_TexCoord);
			
			// uniforms
			mat4 projection = glm::ortho(0.0f, 320.0f, 480.0f, 0.0f);
			glUniformMatrix4fv(u_Projection, 1, false, value_ptr(projection));
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(atlas.texture->get(), 0);
			glUniform1i(u_Tex, 0);
			
			// vertex attrib
			glVertexAttribPointer(a_Pos, 2, GL_FLOAT, false, sizeof(VertexData), &verts[0].pos);
			glVertexAttribPointer(a_TexCoord, 2, GL_FLOAT, false, sizeof(VertexData), &verts[0].uv);
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
			glDrawElements(GL_TRIANGLES, sprites.size()*6, GL_UNSIGNED_SHORT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete[] verts;
		}
	};
}

#endif
