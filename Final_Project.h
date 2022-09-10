#pragma once
#include "resource.h"
#include <iostream>
#include <math.h>
#include <windows.h>
#include <string>

using namespace std;
class Point {
private:
	double x, y;
public:
	Point(double x = 0, double y = 0);
	Point(const Point& k);
	void set(double x, double y);
	void setX(double xx);
	void setY(double yy);
	double GetX() const;
	double GetY() const;
	double Distance(Point other);
	Point MidPoint(Point other);
	virtual void print();
	virtual void draw(HDC hdc, HGDIOBJ original);
	virtual void Move(double dx, double dy);
	virtual void Scale(double T);
	virtual bool isIntersecting(HDC hdc, Point A, Point B);

};

class Linear : public Point {
private:
	double a, b, c; //ax + by = c;
	Point M, N;
public:
	Linear(Point M, Point N);
	Linear(double x1, double y1, double x2, double y2);
	double get_a();
	double get_b();
	double get_c();
	void set(Point M, Point N);
	void set(double Mx, double My, double Nx, double Ny);
	void draw(HDC hdc, HGDIOBJ original);
};

class RecTangle : public Point {
private:
	Point A;
	double d, r;
public:
	RecTangle(double xa, double ya, double d, double r);
	void set(double xa, double ya, double d, double r);
	void draw(HDC hdc, HGDIOBJ original);
	void Move(double dx, double dy);
	void Scale(double T);
	bool isIntersecting(HDC hdc, Point N, Point M);
	
};

class Square :public RecTangle {
private:
	double s;
public:
	Square(double x, double y, double s);
};

class Triangle : public Point {
private:
	Point A, B, C; // 3 dinh tam giac
public:
	Triangle(double xa, double ya, double xb, double yb, double xc, double yc);
	Triangle(Point A, Point B, Point C);
	void set(double xa, double ya, double xb, double yb, double xc, double yc);
	void print();
	void Move(double dx, double dy);
	void draw(HDC hdc, HGDIOBJ original);
	void Scale(double T);
	Point center();
	bool isIntersecting(HDC hdc, Point N, Point M);

};

class Elip : public Point {
private:
	double a, b; // do dai 2 duong cheo
	Point M;
public:
	Elip(double x = 0, double y = 0, double a = 0, double b = 0);
	Elip(Point A, double a = 0, double b = 0);
	double GetA() const;
	double GetB() const;
	void print();
	void draw(HDC hdc, HGDIOBJ original);
	void Move(double dx, double dy);
	void Scale(double T);
	bool isIntersecting(HDC hdc, Point N, Point M);
};

class Circle :public Elip {
private:
	double r;
public:
	Circle(double x = 0, double y = 0, double r = 0);

};

class Crescent : public Point {
private:
	Point Center;
	double r;
public:
	Crescent(double x_c = 0, double y_c = 0, double r = 0);
	void draw(HDC hdc, HGDIOBJ original);
	void Move(double dx, double dy);
	void Scale(double T);
	bool isIntersecting(HDC hdc, Point N, Point M);
};
class ConvexPolygon :public Point {
private:
	Point* p;
	int size;
public:
	ConvexPolygon(double coor[] = NULL, int len = 0);
	~ConvexPolygon();
	void draw(HDC hdc, HGDIOBJ original);
	void Move(double dx = 1, double dy = 1);
	void Scale(double T);
	bool isIntersecting(HDC hdc, Point N, Point M);
	Point Center();
	

};
double DistancePointToLine(Linear L, Point A);
int FindIndex_of_SmallestValue(double a[], int n);
bool Check_2_Lines_Intersect(Linear L, Linear K);
Point Coordinates_of_intersection_of_2_lines(Linear L, Linear K);

void mark(HDC hdc, Point I);
bool check_inLine(Point N, Point M, Point I);

Point NewPoint_afterScale(Point midpoint, Point center, double T);
int Count_Intersect(Point** p, int n, HDC hdc, Point N, Point M);
