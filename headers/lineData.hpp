#pragma once

namespace LineData {
	extern unsigned int count;
	extern float** vertices; //The vertices of a specific line
	extern unsigned int* segmentCount; //The count of segments in a specific line
	extern float maxIntensity;

	void initialize();
	void clear();
	void calculateVertices();
}
