#pragma once

namespace line_data {
	extern unsigned int count;
	extern float** vertices; //The vertices of a specific line
	extern unsigned int* segment_count; //The count of segments in a specific line
	extern float max_intensity;

	void initialize();
	void clear();
	void calculate_vertices();
}
