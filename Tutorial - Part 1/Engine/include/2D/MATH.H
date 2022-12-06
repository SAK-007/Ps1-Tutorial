/*
*
* math.h
*
* Author:SAK-007
*
* Created on:11/27/2022
*
*/
#include<Libmath.h>

float Square(float Squarenum) {
	return Squarenum * Squarenum;
}

float SquareRoot(float x) {
	return sqrt(x);	
}


float Max(float x,float y){
	if (x>y)
		return x;
	else
		return y;
}

float MaxValue(float g, float h, float i) {
	if (g > h && g > i)
		return g;
	if (h > g && h > i)
		return h;
	else
		return i;
}

float MinValue(float X, float Y) {
	if (X < Y)
		return X;
	else
		return Y;
}

float Min(float t, float y, float u) {
	if (t < y&&t < u)
		return t;
	if (y < t&&y < u)
		return y;
	else
		return u;
}

float Power(float Number, float Power) {
	return pow(Number, Power);
}

float AreaOfRectangle(float height, float widht) {
	float area;
	area = height * widht;
	return area;
}

float PerimiterOfRectangle(float height, float widht) {
	float areaTemp;
	areaTemp = height * widht;
	return areaTemp * 2;
}

float AreaOfTriangle(float base, float height) {
	return 0.5 * base * height;
}

float PerimiterOfTriangle(float x, float y, float z) {
	return x + y + z;
}

float AreaOfSquare(float size) {
	return size * size;
}

float PerimiterOfSquare(float size) {
	return size * 4;
}