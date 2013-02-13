//
//  shader.h
//  shogles
//
//  Created by Simon Harvey on 12-10-29.
//
//

#ifndef shogles_shader_h
#define shogles_shader_h

#include <OpenGLES/ES2/gl.h>
#include <regex>
#include <iostream>
#include <list>
#include <map>
#include <vector>

using namespace std;

namespace shogles
{
	class ShaderHandle;
	
	class Shader
	{
		GLuint program;
		list<string> vertex_parts, fragment_parts;
		
	public:
		Shader() {
			program = glCreateProgram();
		}
		
		void add_source(const char *path) {
			FILE *f = fopen(path, "r");
			fseek(f, 0, SEEK_END);
			size_t length = ftell(f);
			fseek(f, 0, SEEK_SET);
			char *buffer = new char[length+1];
			fread(buffer, length, 1, f);
			buffer[length] = '\0';
			fclose(f);
			
			string input = buffer;
			regex parts_regex("@(\\w+)([^@]+)@end");
			const sregex_token_iterator end;
			const int subs[] = {1, 2};
			sregex_token_iterator i(input.begin(), input.end(), parts_regex, subs);
			for (; i != end; i++) {
				string name = *i;
				string content = *(++i);
				if (name.compare("vertex") == 0) {
					vertex_parts.push_back(content);
				} else if (name.compare("fragment") == 0) {
					fragment_parts.push_back(content);
				}
			}
			
			delete[] buffer;
		}
		
		bool compile(const string defines = "") {
			string vertex_src = defines, frag_src = defines;
			for (string &s : vertex_parts) vertex_src.append(s);
			for (string &s : fragment_parts) frag_src.append(s);
			GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src.c_str());
			GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, frag_src.c_str());
			glAttachShader(program, vertex_shader);
			glAttachShader(program, fragment_shader);
			
			GLint success = GL_FALSE;
			glLinkProgram(program);
			
			// todo: clean this shit up
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if(success == GL_FALSE)
			{
				GLchar errorLog[1024] = {0};
				glGetProgramInfoLog(program, 1024, NULL, errorLog);
				cout << "error linking program: " << errorLog;
				return false;
			}
			
			glValidateProgram(program);
			glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
			
			if(success == GL_FALSE)
			{
				GLchar errorLog[1024] = {0};
				glGetProgramInfoLog(program, 1024, NULL, errorLog);
				cout << "error validating shader program; Details: " << errorLog << endl;
				return false;
			}
			
			return success != GL_FALSE;
		}
		
		GLuint attrib_loc(const char *name) const {
			return glGetAttribLocation(program, name);
		}
		
		GLuint uniform_loc(const char *name) const {
			return glGetUniformLocation(program, name);
		}
		
		void use() const {
			glUseProgram(program);
		}
		
	protected:
		
		GLuint compile_shader(GLenum type, const char * source) {
			GLuint shader = glCreateShader(type);
			GLint length = strlen(source);
			
			glShaderSource(shader, 1, (const GLchar **)&source, &length);
			glCompileShader(shader);
			
			GLint result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if(result == GL_FALSE) {
				char *log;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
				log = (char *)malloc(length);
				glGetShaderInfoLog(shader, length, &result, log);
				cout << log << endl;
				free(log);
				glDeleteShader(shader);
				return 0;
			}
			
			return shader;
		}
	};
	
	class ShaderHandle
	{
		vector<const char *>bound_tex;
		
	public:
		const Shader &shader;
		
		ShaderHandle(const Shader &shader) : shader(shader)
		{
			
		}
		
		void bind_tex(const char * uniform_name, GLuint tex)
		{
			GLuint id = bound_tex.size();
			glActiveTexture(id);
			glBindTexture(GL_TEXTURE_2D, tex);
			glUniform1i(shader.uniform_loc(uniform_name), id);
			bound_tex.push_back(uniform_name);
		}
		
		GLuint vertex_attrib_ptr(const char *name)
		{
			GLuint loc = shader.attrib_loc(name);
			glEnableVertexAttribArray(loc);
			return loc;
		}
	};
}

#define ATTRIB(shader, name) GLuint name = shader->attrib_loc(#name)
#define UNIFORM(shader, name) GLuint name = shader->uniform_loc(#name)

#endif
