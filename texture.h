//
//  texture.h
//  Frontline
//
//  Created by Simon Harvey on 12-12-28.
//  Copyright (c) 2012 Simon Harvey. All rights reserved.
//

#ifndef Frontline_texture_h
#define Frontline_texture_h

#include <GLKit/GLKit.h>
#include "shogles_utils.h"
#include <glm/glm.hpp>
#include <iostream>
#include <string>

using namespace std;

namespace shogles
{
	class Texture
	{
	private:
		const string &path;
		GLuint tex_id;
		uint _width, _height;
		
	public:
		Texture(const string &path) : path(path), tex_id(0)
		{
			
		};
		
		GLuint get()
		{
			if (!tex_id) {
				load_texture();
			}
			return tex_id;
		}
		
		glm::vec2 size() {
			if (!tex_id) {
				load_texture();
			}
			return glm::vec2(_width, _height);
		}
		
	private:
		
		void load_texture()
		{
			cout << "load texture\n";
			
			NSString *fullpath = [NSString stringWithUTF8String:path.c_str()];
			NSArray *a = [fullpath componentsSeparatedByString:@"."];
			NSString *name = [a objectAtIndex:0];
			NSString *type = [a objectAtIndex:1];
			NSDictionary * options = [NSDictionary dictionaryWithObjectsAndKeys:
									  [NSNumber numberWithBool:NO],
									  GLKTextureLoaderOriginBottomLeft,
									  nil];
			NSString *namepath = [fullpath stringByDeletingPathExtension];
			NSString *folderpath = [namepath stringByDeletingLastPathComponent];
			
			NSString *uri = [[NSBundle mainBundle] pathForResource:[name lastPathComponent] ofType:type inDirectory:folderpath];
			
			NSError *error;
			GLKTextureInfo *tex = [GLKTextureLoader textureWithContentsOfFile:uri
																	  options:options error:&error];
			
			/*if (error) {
				NSDictionary *userInfo = [error userInfo];
				NSString *error = [[userInfo objectForKey:GLKTextureLoaderErrorKey] localizedDescription];
				NSLog(@"error:%@", error);
			}*/
			
			tex_id = tex.name;
			_width = tex.width, _height = tex.height;
		}
		
		void unload_texture()
		{
			if (tex_id) {
				glDeleteTextures(1, &tex_id);
				tex_id = 0;
			}
		}
	};
}

#endif
