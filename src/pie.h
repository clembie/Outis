#ifndef _PIE
#define _PIE

#include "ofMain.h"
#include <vector>


class pie {
	
public:
	int start;
	int numSlices;
	
	float angle;
	float radius;
	float x;
	float y;
	float sectionAngle;
	
	vector<bool> whichSlices;
	vector<int> reds;
	vector<int> greens;
	vector<int> blues;

	pie();
	~pie();
	pie(float _x, float _y, int n, float r);
	void set(float _x, float _y, int n, float r);
	void update(float _x, float _y, float r);
	void draw(int alpha);
	void line2(float x, float y, float angle, float radius);
	float radians(float d);
	
};

#endif
