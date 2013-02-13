//
//  utils.h
//  Frontline
//
//  Created by Simon Harvey on 12-11-12.
//  Copyright (c) 2012 Simon Harvey. All rights reserved.
//

#ifndef Frontline_utils_h
#define Frontline_utils_h

template <typename T>
T rand_range(T min, T max)
{
	return (rand() % (max-min)) + min;
	//return (max-min)*(rand() / (float)RAND_MAX) + min;
}

#endif
