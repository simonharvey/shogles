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
		NSString *s = [NSString stringWithUTF8String:f];
		NSArray *a = [s componentsSeparatedByString:@"."];
		NSString *name = [a objectAtIndex:0];
		NSString *type = [a objectAtIndex:1];
		NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:type];	  
		return [path UTF8String];
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
}

#endif
