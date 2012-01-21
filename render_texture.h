//
//  render_texture.h
//  Shadows
//
//  Created by Simon Harvey on 12-01-04.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Shadows_render_texture_h
#define Shadows_render_texture_h

#include <OpenGLES/ES2/gl.h>
#include "utils.h"

namespace shogles 
{
	struct RenderTexture
	{
		GLuint framebuffer;
		GLuint tex;
		GLint old_fbo;
		const GLuint width, height;
		
		RenderTexture(GLuint pwidth, GLuint pheight) : width(pwidth), height(pheight)
		{
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
			
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			
			int w = next_pot(width), h = next_pot(height);
			
			void *data = malloc((int)(w * h * 8));
			memset(data, 0, (int)(w * h * 8));
			
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			free(data);
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
			
			/*
			 GLuint depthRenderbuffer;
			 glGenRenderbuffers(1, &depthRenderbuffer);
			 glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
			 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
			*/ 
			
			GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				cout << status << endl;
				exit(status);
			}
			
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			
			glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		}
		
		void begin()
		{
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, width, height);
		}
		
		void end()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		}
	};

}

#endif
