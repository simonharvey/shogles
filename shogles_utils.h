//
//  utils.h
//  Shadows
//
//  Created by Simon Harvey on 12-01-04.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Shadows_utils_h
#define Shadows_utils_h

namespace shogles 
{
	#define deg2rad(x) GLKMathDegreesToRadians(x)
	
	static unsigned long next_pot(unsigned long x)
	{
		x = x - 1;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >>16);
		return x + 1;
	}
	
	inline const char * abs_path(const char *f)
	{
		NSString *fullpath = [NSString stringWithUTF8String:f];
		NSArray *a = [fullpath componentsSeparatedByString:@"."];
		NSString *name = [a objectAtIndex:0];
		NSString *type = [a objectAtIndex:1];
		NSString *namepath = [fullpath stringByDeletingPathExtension];
		NSString *folderpath = [namepath stringByDeletingLastPathComponent];
		
		NSString *uri = nil;
		
		if (![folderpath isEqualToString:@""]) {
			uri = [[NSBundle mainBundle] pathForResource:[name lastPathComponent] ofType:type inDirectory:folderpath];
		} else {
			uri = [[NSBundle mainBundle] pathForResource:name ofType:type];
		}
		
		
		return [uri UTF8String];
	}
	
	inline GLuint load_texture(const char *path)
	{
		NSString *s = [NSString stringWithUTF8String:path];
		NSArray *a = [s componentsSeparatedByString:@"."];
		NSString *name = [a objectAtIndex:0];
		NSString *type = [a objectAtIndex:1];
		NSDictionary * options = [NSDictionary dictionaryWithObjectsAndKeys:
								  [NSNumber numberWithBool:YES],
								  GLKTextureLoaderOriginBottomLeft, 
								  nil];
		GLKTextureInfo *tex = [GLKTextureLoader textureWithContentsOfFile:[[NSBundle mainBundle] pathForResource:name ofType:type] 
																  options:options error:nil];
		GLuint tname = tex.name;
		//[tex release];
		return tname;
	}
	
	template <typename T>
	T *gen_quads_indices(uint num_quads)
	{
		// 0 1
		// 2 3
		
		//  0 2 1 | 1 2 3
		
		T *result = new T[num_quads*6];
		for (int i=0; i<num_quads; ++i) {
			int i0 = i*6;
			result[i0] = i*4;
			result[i0+1] = i*4+2;
			result[i0+2] = i*4+1;
			result[i0+3] = i*4+1;
			result[i0+4] = i*4+2;
			result[i0+5] = i*4+3;
		}
		
		return result;
	}
}

#endif
