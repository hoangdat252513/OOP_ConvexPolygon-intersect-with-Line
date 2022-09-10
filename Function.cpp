#include "Final_Project.h"

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
Point::Point(const Point& k) {
	x = k.GetX();
	y = k.GetY();
}
void Point::set(double x, double y) {
	this->x = x;
	this->y = y;
}
void Point::setX(double xx) {
	x = xx;
}
void Point::setY(double yy) {
	y = yy;
}
double Point::GetX() const {
	return x;
}
double Point::GetY() const {
	return y;
}
double Point::Distance(Point other)
{
	return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}
Point Point::MidPoint(Point other)
{
	return Point((x + other.GetX())/2, (y + other.GetY())/2);
}
void Point::print() {
	cout << "\t(" << x << "," << y << ")" << endl;
}
void Point::draw(HDC hdc, HGDIOBJ original) {}
void Point::Move(double dx, double dy) {
	x += dx;
	y += dy;
}
void Point::Scale(double T) {
	x *= T;
	y *= T;
}

bool Point::isIntersecting(HDC hdc, Point A, Point B)
{
	return false;
}


Linear::Linear(Point M, Point N) {//he so phuong trinh duong thang
	this->a = N.GetY() - M.GetY();
	this->b = M.GetX() - N.GetX();

	this->c = a * M.GetX() + b * M.GetY();
	this->M = M;
	this->N = N;

}
Linear::Linear(double x1, double y1, double x2, double y2)
{
	M = Point(x1, y1);
	N = Point(x2, y2);
	set(Point(x1, y1), Point(x2, y2));

}
double Linear::get_a()
{
	return a;
}
double Linear::get_b()
{
	return b;
}
double Linear::get_c()
{
	return c;
}
void Linear::set(Point M, Point N) {
	this->a = N.GetY() - M.GetY();
	this->b = M.GetX() - N.GetX();
	this->c = M.GetX() * N.GetY() - M.GetY() * N.GetX();
}
void Linear::set(double Mx, double My, double Nx, double Ny) {
	this->a = Ny - My;
	this->b = Mx - Nx;
	this->c = Mx * Ny - My * Nx;
}

void Linear::draw(HDC hdc, HGDIOBJ original) {
	original = SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(255, 0, 0));
	SetDCBrushColor(hdc, RGB(255, 0, 0));

	MoveToEx(hdc, M.GetX(), M.GetY(), NULL);
	LineTo(hdc, N.GetX(), N.GetY());
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
}

RecTangle::RecTangle(double xa, double ya, double d, double r) {
	A.set(xa, ya);
	this->d = d;
	this->r = r;
}
void RecTangle::set(double xa, double ya, double d, double r) {
	A.set(xa, ya);
	this->d = d;
	this->r = r;
}
void RecTangle::draw(HDC hdc, HGDIOBJ original) {
	original = SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
	Linear L(Point(300, 100), Point(500, 300));
	L.draw(hdc, original);

	// 
	if (isIntersecting(hdc, Point(300, 100), Point(500, 300))) {
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, 0, 0));
	}
	Rectangle(hdc, A.GetX(), A.GetY(), A.GetX() + d, A.GetY() + r);
}
void RecTangle::Move(double dx, double dy) {
	A.setX(A.GetX() + dx);
	A.setY(A.GetY() + dy);
}
void RecTangle::Scale(double T) {
	d *= T;
	r *= T;
}

bool RecTangle::isIntersecting(HDC hdc, Point N, Point M)
{
	// Line is created by 2 points MN.
	// ax + by = c
	Linear NM(N, M);


	// Ox ---------------------
		// A	B
		// D	C
	/*Point center(A.GetX() + d / 2, A.GetY() + r / 2);
	double t = DistancePointToLine(NM, center);
	double diag = sqrt(pow(d, 2) + pow(r, 2));
	if (t <= r / 2)
		return true;
	else {
		if (t > diag / 2)
			return false;
	}
	*/
	Point B(A.GetX() + d, A.GetY());
	Point C(A.GetX() + d, A.GetY() + r);
	Point D(A.GetX(), A.GetY() + r);


	//	Find the minimun distance from 4 vertices of rectangle ABCD to this line
	double distance_Point_to_Line[4];
	char list_vertices[] = { 'A', 'B','C', 'D' };
	Point p[] = { A, B, C, D };
	for (int i = 0; i < 4; i++) {
		distance_Point_to_Line[i] = DistancePointToLine(NM, p[i]);
		// if the distance of one of each vertex to this line is 0, it will lie on this line. so that, it is intersecting.
		if (distance_Point_to_Line[i] == 0) {
			return true;
		}
	}

	char vertex_nearest_line = list_vertices[FindIndex_of_SmallestValue(distance_Point_to_Line, 4)];
	switch (vertex_nearest_line) {
	case 'A': {
		// consider the intersection of this line and AB, AD
		Linear AB(A, B);
		Linear AD(A, D);
		if (Check_2_Lines_Intersect(AB, NM) || Check_2_Lines_Intersect(AD, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(AB, NM);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(AD, NM);
			mark(hdc, J);
			if (check_inLine(A, B, I) || check_inLine(A, D, J))
				return true;
		}
		//return false;

	} break;
	case 'B': {
		// consider the intersection of this line and BC, BA
		Linear BC(B, C);
		Linear BA(B, A);
		if (Check_2_Lines_Intersect(BC, NM) || Check_2_Lines_Intersect(BA, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(BC, NM);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(BA, NM);
			mark(hdc, J);
			if (check_inLine(B, C, I) || check_inLine(B, A, J))
				return true;
		}
		//return false;
	} break;
	case 'C': {
		// consider the intersection of this line and CD, CB
		Linear CD(C, D);
		Linear CB(C, B);
		if (Check_2_Lines_Intersect(CD, NM) || Check_2_Lines_Intersect(CB, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(CD, NM);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(CB, NM);
			mark(hdc, J);
			if (check_inLine(C, D, I) || check_inLine(C, B, J))
				return true;
		}
		//return false;
	} break;
	case 'D': {
		// consider the intersection of this line and DA, DC
		Linear DA(D, A);
		Linear DC(D, C);
		if (Check_2_Lines_Intersect(DA, NM) || Check_2_Lines_Intersect(DC, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(DA, NM);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(DC, NM);
			mark(hdc, J);
			if (check_inLine(D, A, I) || check_inLine(D, C, J))
				return true;
		}
		//return false;
	} break;
	}

	return false;
}

Square::Square(double x, double y, double s) :RecTangle(x, y, s, s) {
	this->s = s;
}

Triangle::Triangle(double xa, double ya, double xb, double yb, double xc, double yc) {
	A.set(xa, ya);
	B.set(xb, yb);
	C.set(xc, yc);
}
Triangle::Triangle(Point A, Point B, Point C):A(A), B(B), C(C){}
void Triangle::set(double xa, double ya, double xb, double yb, double xc, double yc) {
	A.set(xa, ya);
	B.set(xb, yb);
	C.set(xc, yc);
}
void Triangle::print() {
	double x = A.Distance(B), y = B.Distance(C), z = A.Distance(C);
	cout << "Triangle: x = " << x << ", y = " << y << ", z = " << z << ", 3 Point: ";
	A.print(); B.print(); C.print();
}
void Triangle::Move(double dx, double dy) {
	A.Move(dx, dy);
	B.Move(dx, dy);
	C.Move(dx, dy);
}
void Triangle::draw(HDC hdc, HGDIOBJ original) {
	original = SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
	Linear L(Point(300, 100), Point(500, 300));
	L.draw(hdc, original);

	// 
	if (isIntersecting(hdc, Point(300, 100), Point(500, 300))) {
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, 0, 0));
	}
	/*MoveToEx(hdc, A.GetX(), A.GetY(), NULL);
	LineTo(hdc, B.GetX(), B.GetY());
	LineTo(hdc, C.GetX(), C.GetY());
	LineTo(hdc, A.GetX(), A.GetY());
	*/
	POINT apt[] = { A.GetX(),A.GetY(),B.GetX(),B.GetY(),C.GetX(),C.GetY() };
	Polygon(hdc, apt, 3);
	
}

void Triangle::Scale(double T)
{
	Point I = A.MidPoint(B);
	Point J = B.MidPoint(C);
	Point K = C.MidPoint(A);
	Point G = this->center();
// GA' = T*GA = T*2*IG
// Vector GA' = 2*T*vector(IG) => coordinates of new scale point
	C = NewPoint_afterScale(I, G, T);
	A = NewPoint_afterScale(J, G, T);
	B = NewPoint_afterScale(K, G, T);
}

Point Triangle::center()
{
	return Point((A.GetX() + B.GetX() + C.GetX()) / 3, (A.GetY() + B.GetY() + C.GetY()) / 3);
}
 
bool Triangle::isIntersecting(HDC hdc, Point N, Point M)
{
	Linear NM(N, M);
	double distance_Point_to_Line[3];
	char list_vertices[] = { 'A', 'B','C' };
	Point p[] = { A, B, C };
	for (int i = 0; i < 3; i++) {
		distance_Point_to_Line[i] = DistancePointToLine(NM, p[i]);
		// if the distance of one of each vertex to this line is 0, it will lie on this line. so that, it is intersecting.
		if (distance_Point_to_Line[i] == 0 ) return true;
	}

	char vertex_nearest_line = list_vertices[FindIndex_of_SmallestValue(distance_Point_to_Line, 3)];
	switch (vertex_nearest_line) {
	case 'A': {
		// consider the intersection of this line and AB, AC
		Linear AB(A, B);
		Linear AC(A, C);
		if (Check_2_Lines_Intersect(AB, NM) || Check_2_Lines_Intersect(AC, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(AB, NM);
			double IA = I.Distance(A);//
			double IB = I.Distance(B);//
			double ab = A.Distance(B);//
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(AC, NM);
			double JA = J.Distance(A);//
			double JC = J.Distance(C);//
			double ac = A.Distance(C);//
			mark(hdc, J);
			if (check_inLine(A, B, I) || check_inLine(A, C, J)) return true;
		}
		return false;
	} break;
	case 'B': {
		// consider the intersection of this line and BC, BA
		Linear BC(B, C);
		Linear BA(B, A);
		if (Check_2_Lines_Intersect(BC, NM) || Check_2_Lines_Intersect(BA, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(BC, NM);
			double IB = I.Distance(B);
			double IC = I.Distance(C);
			double bc = B.Distance(C);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(BA, NM);
			double JB = J.Distance(B);
			double JA = J.Distance(A);
			double ba = B.Distance(A);
			mark(hdc, J);
			if (check_inLine(B, C, I) || check_inLine(B, A, J)) return true;
		}
		return false;
	} break;
	case 'C': {
		// consider the intersection of this line and CA, CB
		Linear CA(C, A);
		Linear CB(C, B);
		if (Check_2_Lines_Intersect(CA, NM) || Check_2_Lines_Intersect(CB, NM)) {
			Point I = Coordinates_of_intersection_of_2_lines(CA, NM);
			double IC = I.Distance(C);
			double IA = I.Distance(A);
			double ca = C.Distance(A);
			mark(hdc, I);
			Point J = Coordinates_of_intersection_of_2_lines(CB, NM);
			double JC = J.Distance(C);
			double JB = J.Distance(B);
			double cb = C.Distance(B);
			mark(hdc, J);
			if (check_inLine(C, A, I) || check_inLine(C, B, J)) return true;
		}
		return false;
	} break;
	}
	return false;
}

Elip::Elip(double x, double y, double a, double b) {
	Point A(x, y);
	M = A;
	this->a = a;
	this->b = b;
}
Elip::Elip(Point A, double a, double b) {
	M = A;
	this->a = a;
	this->b = b;
}
double Elip::GetA() const {
	return a;
}
double Elip::GetB() const {
	return b;
}
void Elip::print() {
	cout << a << " " << b << " ";
	M.print();
}
void Elip::draw(HDC hdc, HGDIOBJ original) {
	original = SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
	Linear L(Point(300, 100), Point(500, 300));
	L.draw(hdc, original);

	// 
	if (isIntersecting(hdc, Point(300, 100), Point(500, 300))) {
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, 0, 0));
	}
	Ellipse(hdc, M.GetX() - a, M.GetY() - b, M.GetX() + a, M.GetY() + b);
}
void Elip::Move(double dx, double dy) {
	M.setX(M.GetX() + dx);
	M.setY(M.GetY() + dy);
}
void Elip::Scale(double T) {
	a *= T;
	b *= T;
}

bool Elip::isIntersecting(HDC hdc, Point N, Point M)
{//suppose that elip equation is (x-x0)^2/a^2 + (y-y0)^2/b^2 = 1 with center(x0;y0)
	// replace y by y = (-a/b)x +c/a (from ax +by = c) into elip
	// the equation is a quadratic equation in x: Ax^2 + Bx + C = 0 it can have at least one roots
	// so we consider delta = B^2 -4AC >=0 from quadratic equation we have
	Linear NM(N, M);
	double factor_A = pow(this->b, 2) + pow(this->a * NM.get_a() / NM.get_b(), 2),
		factor_B = -2 * this->M.GetX() * pow(this->b, 2) - 2 * pow(this->a, 2) * (NM.get_a() / NM.get_b()) * (NM.get_c() / NM.get_b() - this->M.GetY()),
		factor_C = pow(this->M.GetX() * this->b, 2) + pow(this->a, 2) * pow(NM.get_c() / NM.get_b() - this->M.GetY(), 2) - pow(this->a * this->b, 2);

	if (pow(factor_B, 2) - 4 * factor_A * factor_C >= 0) return true;
	return false;
}

Circle::Circle(double x, double y, double r) :Elip(x, y, r, r) {
	this->r = r;
}

Crescent::Crescent(double x_c, double y_c, double r )
{
	Center = Point(x_c, y_c);
	this->r = r;
}

void Crescent::draw(HDC hdc, HGDIOBJ original) {
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
	Linear L(Point(300, 100), Point(500, 300));
	L.draw(hdc, original);

	if (isIntersecting(hdc, Point(300, 100), Point(500, 300))) {
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, 0, 0));
	}
	Pie(hdc, Center.GetX() - r, Center.GetY() - r, Center.GetX() + r, Center.GetY() + r, Center.GetX() - r, Center.GetY(), Center.GetX() + r, Center.GetY());
}

void Crescent::Move(double dx, double dy)
{
	Center.Move(dx, dy);
}

void Crescent::Scale(double T)
{
	r *= T;
}

bool Crescent::isIntersecting(HDC hdc, Point N, Point M)
{
	Elip elip(Center, r, r);
	// Diameter AB
	Point A(Center.GetX() - r, Center.GetY());
	Point B(Center.GetX() + r, Center.GetY());
	Linear AB(A,B);
	Linear NM(N, M);

	if (Check_2_Lines_Intersect(AB, NM)) {
		Point I = Coordinates_of_intersection_of_2_lines(AB, NM);
		mark(hdc, I);
		if (check_inLine(A, B, I) == true)
			return true;
		else {
			if (I.GetX() < Center.GetX() && elip.isIntersecting(hdc, N, M))
				return true;
		}
	}
	return false;
}

ConvexPolygon::ConvexPolygon(double coor[], int len) {
	this->size = len/2;
	p = new Point[size];
	for (int i = 0, j =0 ; i < size*2 && j< size;j++, i += 2) {
		p[j].set(coor[i], coor[i + 1]);
	}
}
ConvexPolygon::~ConvexPolygon() {
	delete[]p;
}

void ConvexPolygon::draw(HDC hdc, HGDIOBJ original)
{
	SetDCPenColor(hdc, RGB(0, 255, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, RGB(0, 0, 255));
	Linear L(Point(300, 100), Point(500, 300));
	L.draw(hdc, original);
	
	if (isIntersecting(hdc, Point(300, 100), Point(500, 300))) {
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCBrushColor(hdc, RGB(255, 0, 0));
	}

	POINT* apt = new POINT[size];
	for (int i = 0; i < size; i++) {
		apt[i].x = p[i].GetX();
		apt[i].y = p[i].GetY();
	}

	Polygon(hdc, apt, size);

}

void ConvexPolygon::Move(double dx, double dy)
{
	for (int i = 0; i < size; i++) {
		p[i].Move(dx, dy);
	}
}

void ConvexPolygon::Scale(double T)
{
	Point center = Center();
	for (int i = 0; i < size; i++) {
		p[i].setX(T * (p[i].GetX() - center.GetX()) + center.GetX());
		p[i].setY(T * (p[i].GetY() - center.GetY()) + center.GetY());
	}
}

bool ConvexPolygon::isIntersecting(HDC hdc, Point N, Point M)
{
	Linear NM(N, M);


	// Find the smallest element in Point p array to NM
	int i = 0;
	int index = 0;
	double distance = INT_MAX;
	while (index < size) {
		if (distance > DistancePointToLine(NM, p[index])) {
			distance = DistancePointToLine(NM, p[index]);
			i = index;
		}
		index++;
	}
	int j = 0;
	int k = 0;
	if (i >= size - 1) {
		j = 0;
	}
	else {
		j = i + 1;
	}
	if (i <= 0) {
		k = size - 1;
	}
	else {
		k = i - 1;
	}






	Linear AB(p[i], p[j]);
	Linear AD(p[i], p[k]);
	if (Check_2_Lines_Intersect(AB, NM) || Check_2_Lines_Intersect(AD, NM)) {
		Point I = Coordinates_of_intersection_of_2_lines(AB, NM);
		mark(hdc, I);
		Point J = Coordinates_of_intersection_of_2_lines(AD, NM);
		mark(hdc, J);
		if (check_inLine(p[i], p[j], I) || check_inLine(p[i], p[k], J))
			return true;
	}



	return false;
}

Point ConvexPolygon::Center()
{
	double x = 0;
	double y = 0;
	for (int i = 0; i < size; i++) {
		x += p[i].GetX();
		y += p[i].GetY();
	}

	return Point(x/size, y/size);
}



double DistancePointToLine(Linear L, Point A)
{
	double numerator = abs(A.GetX() * L.get_a() + A.GetY() * L.get_b() - L.get_c());
	double denominator = sqrt(pow(L.get_a(), 2) + pow(L.get_b(), 2));
	return numerator / denominator;
}

int FindIndex_of_SmallestValue(double a[], int n)
{
	int index = 0;
	double min = a[index];
	for (int i = 1; i < n; i++) {
		if (a[i] <= a[index])
			index = i;
	}
	return index;
}

bool Check_2_Lines_Intersect(Linear L, Linear K)
{
	// use cramer's rule to solve it
	// (L): ax + by = c
	// (K): a'x + b'y = c
	double d = L.get_a() * K.get_b() - K.get_a() * L.get_b();
	double dx = L.get_c() * K.get_b() - K.get_c() * L.get_b();
	double dy = L.get_a() * K.get_c() - K.get_a() * L.get_c();
	if (d != 0)
		return true;
	return false;
}

Point Coordinates_of_intersection_of_2_lines(Linear L, Linear K)
{
	// use cramer's rule to solve it
	// (L): ax + by = c
	// (K): a'x + b'y = c
	double d = L.get_a() * K.get_b() - K.get_a() * L.get_b();
	double dx = L.get_c() * K.get_b() - K.get_c() * L.get_b();
	double dy = L.get_a() * K.get_c() - K.get_a() * L.get_c();
	return Point(dx / d, dy / d);
}

void mark(HDC hdc, Point I) {
	SetPixel(hdc, I.GetX(), I.GetY(), RGB(255, 0, 0));
}

bool check_inLine(Point N, Point M, Point I)
{
	return  abs (I.Distance(N) + I.Distance(M) - N.Distance(M) ) <= 0.0000001;
}

Point NewPoint_afterScale(Point midpoint, Point center, double T)
{
	return Point(2*T*(center.GetX() - midpoint.GetX()) + center.GetX(), 
		2 * T * (center.GetY() - midpoint.GetY()) + center.GetY());
}

int Count_Intersect(Point** p, int n,HDC hdc, Point N, Point M) {
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (p[i]->isIntersecting(hdc, N, M)) {
			count++;
		}
	}
	return count;
}