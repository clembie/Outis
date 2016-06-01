

#include "pie.h"
#include "radialPatch.h"


radialPatch::radialPatch() {
	numSlices = 12;
	numPies = 5;
	radius = 100;
	bandSize = radius/numPies;
	x = 200;
	y = 200;
	
	for(int i = 0; i < numPies; i++) {
		pies.push_back(pie(x, y, numSlices, radius+bandSize-((i+1)*bandSize)));
	}
}

radialPatch::~radialPatch() {
	// destructor...
}

radialPatch::radialPatch(float _x, float _y, int ns, int np, float _radius) {
	numSlices = ns;
	numPies = np;
	radius = _radius;
	bandSize = radius/numPies;
	x = _x;
	y = _y;
		
	for(int i = 0; i < numPies; i++) {
		pies.push_back(pie(_x, _y, numSlices, radius+bandSize-((i+1)*bandSize)));
	}
}

void radialPatch::set(int pie, int slice, int col) {
	// fault tolerance
	if( slice > numSlices-1 ) slice = numSlices-1;
	if( slice < 0 ) slice = 0;
	if( pie > numPies-1 ) pie = numPies-1;
	if( pie < 0 ) pie = 0;
	
	if(col < 0) {
		pies[pie].whichSlices[slice] = false;
		pies[pie].reds[slice] = 255;
		pies[pie].greens[slice] = 255;
		pies[pie].blues[slice] = 255;
	}
	else if(col == 0) {
		pies[pie].whichSlices[slice] = true;
		pies[pie].reds[slice] = 255;
		pies[pie].greens[slice] = 0;
		pies[pie].blues[slice] = 0;
	}
	else if(col == 1) {
		pies[pie].whichSlices[slice] = true;
		pies[pie].reds[slice] = 0;
		pies[pie].greens[slice] = 255;
		pies[pie].blues[slice] = 0;
	}
	else {
		pies[pie].whichSlices[slice] = true;
		pies[pie].reds[slice] = 0;
		pies[pie].greens[slice] = 0;
		pies[pie].blues[slice] = 255;
	}
}

void radialPatch::update(float _x, float _y, float _radius) {
	bandSize = _radius/numPies;
	for(int i = 0; i < numPies; i++) {
		pies[i].update(_x, _y, _radius+bandSize-((i+1)*bandSize));
	}
}
							  
							  
void radialPatch::draw(int alpha) {
	for(int i = 0; i < numPies; i++) {
		pies[i].draw(alpha);
	}
}
