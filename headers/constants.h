#pragma once

const float TWO_PI = 2 * 3.14159265358979323846f;

const unsigned int SCREEN_WIDTH = 900;
const unsigned int SCREEN_HEIGHT = 600;
const float MIN_DELTA_TIME = 1.0f / 60;

const float LINE_ANGLE_OFFSET = 0.0f;
const unsigned int LINE_SEGMENT_PIECES = 10;
const float LINE_PIECE_LENGTH = 0.002f / LINE_SEGMENT_PIECES;

const unsigned int MAX_LINE_SEGMENTS_NORMAL = 1000;
const unsigned int MAX_LINE_SEGMENTS_EXTENDED = MAX_LINE_SEGMENTS_NORMAL * 100;

const float CHARGE_RADIUS = 0.01f;
const float CHARGE_COLORED_RADIUS = 0.008f;

const float CAMERA_ZOOM = 0.6f;
const float CAMERA_MOVE = 0.75f;
