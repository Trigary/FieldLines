#include "lineData.hpp"
#include "chargeData.hpp"

namespace LineData {
	unsigned int calculateLine(unsigned int line, Vec2 position);
	void addToVertexArray(float* vertexArray, unsigned int piece, Vec2 position, float length2);

	float positiveChargeSum = 0.0f;
	unsigned int maxLineSegments = MAX_LINE_SEGMENTS_NORMAL;
	unsigned int count = 0;
	float** vertices = new float*[0];
	unsigned int* segmentCount = new unsigned int[0];
	float maxIntensity = 0;
	


	void initialize() {
		clear();

		float chargeSum = 0.0f;
		unsigned int positiveCharges = 0;
		for (unsigned int i = 0; i < ChargeData::count; i++) {
			float charge = ChargeData::charges[i].charge;
			chargeSum += charge;
			if (charge > 0) {
				positiveChargeSum += charge;
				positiveCharges++;
			}
		}

		if (chargeSum <= 0) {
			maxLineSegments = MAX_LINE_SEGMENTS_EXTENDED;
			printf("extended");
		} else {
			printf("normal");
		}

		for (unsigned int i = 0; i < ChargeData::count; i++) {
			float charge = ChargeData::charges[i].charge;
			if (charge > 0) {
				count += (unsigned int)(charge * PREFERRED_LINE_COUNT / positiveChargeSum);
			}
		}

		vertices = new float*[count];
		segmentCount = new unsigned int[count];
	}

	void clear() {
		for (unsigned int i = 0; i < count; i++) {
			free(vertices[i]);
		}
		delete[] vertices;
		delete[] segmentCount;

		positiveChargeSum = 0.0f;
		maxLineSegments = MAX_LINE_SEGMENTS_NORMAL;
		count = 0;
		maxIntensity = 0;
	}

	void calculateVertices() {
		initialize();

		unsigned int line = 0;
		for (unsigned int chargeIndex = 0; chargeIndex < ChargeData::count; chargeIndex++) {
			Charge charge = ChargeData::charges[chargeIndex];
			if (charge.charge <= 0) {
				continue;
			}

			unsigned int count = (unsigned int)(charge.charge * PREFERRED_LINE_COUNT / positiveChargeSum);
			for (unsigned int i = 1; i <= count; i++) {
				float angle = TWO_PI * i / count + LINE_ANGLE_OFFSET;
				Vec2 position = Vec2(cos(angle), sin(angle));
				position *= CHARGE_RADIUS - LINE_PIECE_LENGTH;
				position += charge.position;
				segmentCount[line] = calculateLine(line, position);
				line++;
			}
		}

		float logNormalizer = log2(maxIntensity);
		for (line = 0; line < count; line++) {
			float* array = vertices[line];
			for (unsigned int i = 0; i < segmentCount[line]; i++) {
				unsigned int index = i * 3 + 2;
				array[index] = log2(array[index]) / logNormalizer;
			}
		}
	}



	unsigned int calculateLine(unsigned int line, Vec2 position) {
		float* vertexArray = (float*)malloc(maxLineSegments * 3 * sizeof(float));
		unsigned int segments = 0;
		float length = 0.0f;
		for (unsigned int piece = 0; piece <= (maxLineSegments - 1) * LINE_SEGMENT_PIECES; piece++) {
			Vec2 fieldSum = Vec2(0.0f, 0.0f);

			for (unsigned int chargeIndex = 0; chargeIndex < ChargeData::count; chargeIndex++) {
				Charge charge = ChargeData::charges[chargeIndex];
				Vec2 field = position - charge.position;

				float distance = field.length();
				if (distance <= CHARGE_RADIUS && charge.charge < 0) {
					if (piece != 0 && piece % LINE_SEGMENT_PIECES != 1) {
						addToVertexArray(vertexArray, segments++, position, length);
					}

					void* pointer = realloc(vertexArray, segments * 3 * sizeof(float));
					if (pointer != nullptr) {
						vertices[line] = (float*)pointer;
					} else {
						vertices[line] = vertexArray;
					}
					printf("realloc");
					return segments;
				}

				field.normalize();
				field *= charge.charge / distance;
				fieldSum += field;
			}

			length = fieldSum.length();
			if (maxIntensity < length) {
				maxIntensity = length;
			}

			fieldSum.normalize();
			fieldSum *= LINE_PIECE_LENGTH;
			position += fieldSum;

			if (piece % LINE_SEGMENT_PIECES == 0) {
				addToVertexArray(vertexArray, segments++, position, length);
			}
		}

		vertices[line] = vertexArray;
		return segments;
	}

	void addToVertexArray(float* vertexArray, unsigned int segment, Vec2 position, float length) {
		unsigned int index = segment * 3;
		vertexArray[index++] = position.x;
		vertexArray[index++] = position.y;
		vertexArray[index] = length;
	}
}
