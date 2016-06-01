#ifndef _RADIALPATCH
#define _RADIALPATCH

#import "pie.h"
#import "ofMain.h"
#import <vector>

class radialPatch {
public:
	vector<pie> pies;
	int numSlices;
	int numPies;
	float radius;
	float bandSize;
	float x;
	float y;
	
	radialPatch();
	~radialPatch();
	radialPatch(float _x, float _y, int ns, int np, float _radius);
	void set(int pie, int slice, int col);
	void update(float _x, float _y, float _radius);
	void draw(int alpha);
};

#endif
