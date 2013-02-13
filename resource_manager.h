//
//  resource_manager.h
//  Frontline
//
//  Created by Simon Harvey on 12-12-28.
//  Copyright (c) 2012 Simon Harvey. All rights reserved.
//

#ifndef Frontline_resource_manager_h
#define Frontline_resource_manager_h

#include <memory>
#include <map>
#include <string>
#include "texture.h"
#include "shader.h"
#include "shogles_utils.h"

using namespace std;

namespace shogles
{
	typedef shared_ptr<Texture> TexturePtr;
	typedef shared_ptr<Shader> ShaderPtr;
	
	class ResourceManager
	{
		map<string, weak_ptr<Texture>> m_textures;
		map<string, weak_ptr<Shader>> m_shaders;
		
	public:
		TexturePtr get_texture(const string &path)
		{
			const auto &it = m_textures.find(path);
			if (it == m_textures.end() || (*it).second.expired()) {
				auto tex = make_shared<Texture>(path);
				m_textures[path] = tex;
				return tex;
			} else {
				return (*it).second.lock();
			}
		}
		
		ShaderPtr get_shader(const string &path)
		{
			const auto &it = m_shaders.find(path);
			if (it == m_shaders.end() || (*it).second.expired()) {
				auto shader = make_shared<Shader>();
				shader->add_source(abs_path(path.c_str()));
				shader->compile();
				m_shaders[path] = shader;
				return shader;
			} else {
				return (*it).second.lock();
			}
		}
		
		static ResourceManager &get_instance()
		{
			static ResourceManager instance;
			return instance;
		}
	};
}

#endif
