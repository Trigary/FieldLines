#include "line_data.hpp"
#include <cmath>
#include "constants.h"
#include "configuration.hpp"
#include "charge_data.hpp"

namespace line_data {
	unsigned int calculate_line(unsigned int line, vec2 position);
	void add_to_vertex_array(float* vertex_array, unsigned int segment, vec2 position, float length);

	float positive_charge_sum = 0.0f;
	unsigned int max_line_segments = MAX_LINE_SEGMENTS_NORMAL;
	unsigned int count = 0;
	float** vertices = new float*[0];
	unsigned int* segment_count = new unsigned int[0];
	float max_intensity = 0;



	void initialize() {
		clear();

		float charge_sum = 0.0f;
		unsigned int positive_charges = 0;
		for (unsigned int i = 0; i < charge_data::count; i++) {
			float charge = charge_data::charges[i].amount;
			charge_sum += charge;
			if (charge > 0) {
				positive_charge_sum += charge;
				positive_charges++;
			}
		}

		if (charge_sum <= 0) {
			max_line_segments = MAX_LINE_SEGMENTS_EXTENDED;
		}

		for (unsigned int i = 0; i < charge_data::count; i++) {
			float charge = charge_data::charges[i].amount;
			if (charge > 0) {
				count += (unsigned int)(charge * preferred_line_count / positive_charge_sum);
			}
		}

		vertices = new float*[count];
		segment_count = new unsigned int[count];
	}

	void clear() {
		for (unsigned int i = 0; i < count; i++) {
			free(vertices[i]);
		}
		delete[] vertices;
		delete[] segment_count;

		positive_charge_sum = 0.0f;
		max_line_segments = MAX_LINE_SEGMENTS_NORMAL;
		count = 0;
		max_intensity = 0;
	}

	void calculate_vertices() {
		initialize();

		unsigned int line = 0;
		for (unsigned int charge_index = 0; charge_index < charge_data::count; charge_index++) {
			charge charge = charge_data::charges[charge_index];
			if (charge.amount <= 0) {
				continue;
			}

			unsigned int count = (unsigned int)(charge.amount * preferred_line_count / positive_charge_sum);
			for (unsigned int i = 1; i <= count; i++) {
				float angle = TWO_PI * i / count + LINE_ANGLE_OFFSET;
				vec2 position = vec2(cos(angle), sin(angle));
				position *= CHARGE_RADIUS - LINE_PIECE_LENGTH;
				position += charge.position;
				segment_count[line] = calculate_line(line, position);
				line++;
			}
		}

		float log_normalizer = log2(max_intensity);
		for (line = 0; line < count; line++) {
			float* array = vertices[line];
			for (unsigned int i = 0; i < segment_count[line]; i++) {
				unsigned int index = i * 3 + 2;
				array[index] = log2(array[index]) / log_normalizer;
			}
		}
	}



	unsigned int calculate_line(unsigned int line, vec2 position) {
		float* vertex_array = (float*)malloc(max_line_segments * 3 * sizeof(float));
		unsigned int segments = 0;
		float length = 0.0f;
		for (unsigned int piece = 0; piece <= (max_line_segments - 1) * LINE_SEGMENT_PIECES; piece++) {
			vec2 field_sum = vec2(0.0f, 0.0f);

			for (unsigned int charge_index = 0; charge_index < charge_data::count; charge_index++) {
				charge charge = charge_data::charges[charge_index];
				vec2 field = position - charge.position;

				float distance = field.length();
				if (distance <= CHARGE_RADIUS && charge.amount < 0) {
					if (piece != 0 && piece % LINE_SEGMENT_PIECES != 1) {
						add_to_vertex_array(vertex_array, segments++, position, length);
					}

					void* pointer = realloc(vertex_array, segments * 3 * sizeof(float));
					if (pointer != nullptr) {
						vertices[line] = (float*)pointer;
					} else {
						vertices[line] = vertex_array;
					}
					return segments;
				}

				field.normalize();
				field *= charge.amount / distance;
				field_sum += field;
			}

			length = field_sum.length();
			if (max_intensity < length) {
				max_intensity = length;
			}

			field_sum.normalize();
			field_sum *= LINE_PIECE_LENGTH;
			position += field_sum;

			if (piece % LINE_SEGMENT_PIECES == 0) {
				add_to_vertex_array(vertex_array, segments++, position, length);
			}
		}

		vertices[line] = vertex_array;
		return segments;
	}

	void add_to_vertex_array(float* vertex_array, unsigned int segment, vec2 position, float length) {
		unsigned int index = segment * 3;
		vertex_array[index++] = position.x;
		vertex_array[index++] = position.y;
		vertex_array[index] = length;
	}
}
