//
//  rect.h
//  Mesh
//
//  Created by Simon Harvey on 12-01-17.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Mesh_rect_h
#define Mesh_rect_h

namespace shogles
{
	template <typename T>
	struct Point
	{
		T x, y;
	};

	template <typename T>
	struct Rect 
	{
		Point<T> a, b, c, d;
		T width() { return abs(d.x - a.x); }
		T height() { return abs(b.y - a.y); }
		T x() { return a.x; }
		T y() { return a.y; }
	};

	template <typename T>
	Rect<T> make_rect(T x, T y, T w, T h)
	{
		Rect<T> r = {
			{x, y+h},
			{x, y},
			{x+w, y},
			{x+w, y+h},
		};
		return r;
	}
}

#endif
