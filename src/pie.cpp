
#include "pie.h"
#include "math.h"

using namespace std;

pie::pie() {
	numSlices = 12;
	angle = 0;
	radius = 100;
	start = 0;
	sectionAngle = (float) (360.0/numSlices);
	x = 200;
	y = 200;
	
	for(int i = 0; i < numSlices - 1; i++) {
		whichSlices.push_back(false);
		reds.push_back(0);
		greens.push_back(0);
		blues.push_back(0);
	}
}

pie::~pie() {
	// destructor...
}

pie::pie(float _x, float _y, int n, float r) {
	numSlices = n;
	angle = 0;
	radius = r;
	start = 0;
	sectionAngle = (float) (360.0/numSlices);
	x = _x;
	y = _y;
	
	for(int i = 0; i < numSlices; i++) {
		whichSlices.push_back(false);
		reds.push_back(0);
		greens.push_back(0);
		blues.push_back(0);
	}
}

void pie::set(float _x, float _y, int n, float r) {
	numSlices = n;
	radius = r;
	sectionAngle = (float) (360.0/numSlices);
	x = _x;
	y = _y;
	
	for(int i = 0; i < numSlices; i++) {
		whichSlices.erase( whichSlices.begin() + i );
		reds.erase( reds.begin() + i );
		greens.erase( greens.begin() + i );
		blues.erase( blues.begin() + i );
		
		whichSlices[i] = false;
		reds[i] = 0;
		greens[i] = 0;
		blues[i] = 0;
	}
}

void pie::update(float _x, float _y, float r) {
	x = _x;
	y = _y;
	radius = r;
}

void pie::draw(int alpha) {
	ofFill();
	ofSetColor(255, 255, 255, alpha);
	
	// circle
	ofCircle(x, y, radius);
	
	// fills
	for(int i = 0; i < numSlices; i++ ) {
		if(whichSlices[i]) {
			ofSetColor(reds[i], greens[i], blues[i], alpha);
			angle = i*sectionAngle;
			float end = (i*sectionAngle)+sectionAngle;
			while(angle < end) {
				line2(x, y, angle, (radius)-1);
				angle = (float) (angle + 0.25);
			}
		}
	}
	
	ofNoFill();
	ofSetColor(255, 255, 255, alpha);
	for(int i = 0; i < 12; i++) {
		line2(x, y, sectionAngle*i, radius);
	}
}

float pie::radians(float d) {
	return ((d * PI) / 180.0);
}

void pie::line2(float x, float y, float angle, float radius) {
	ofLine(x,y,(x + sin(radians(angle))*radius),(y - cos(radians(angle))*radius));
}
