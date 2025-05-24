#include <iostream>
#include "raylib.h"
#include "Vector2D.h"


Vector2D::Vector2D() { x = y = 0; deg = 0.0f; rad = 0; }
Vector2D::Vector2D(float _x, float _y) { x = _x; y = _y; deg = 0; rad = 0; }
Vector2D::Vector2D(const Vector2D& vector) { x = vector.x; y = vector.y;  deg = vector.deg; rad = vector.rad;}
Vector2D::~Vector2D() = default;

double Vector2D::getRad() const { return rad; }
float Vector2D::getDeg() const { return deg; }

void Vector2D::changeOrientationDeg(float deg) {
  this->deg = deg;
  this->rad = (PI / 180.0) * this->deg;
}

void Vector2D::changeOrientationRad(double rad) {
  this->rad = rad;
  this->deg = (float)(PI / 180.0) * this->rad;
}


float Vector2D::getX() const { return x; }
float Vector2D::getY() const { return y; }
void Vector2D::setX(float _x) { x = _x; }
void Vector2D::setY(float _y) { y = _y; }
void Vector2D::setXY(float _x, float _y) { x = _x; y = _y; }
float Vector2D::magnitude() const { return sqrt(x * x + y * y); }

bool Vector2D::operator==(const Vector2D& vector) const { return x == vector.x && y == vector.y; }
bool Vector2D::operator!=(const Vector2D& vector) const { return !(*this == vector); }
bool Vector2D::operator<(const Vector2D& vector) const {
    return this->magnitude() < vector.magnitude();
}
bool Vector2D::operator>(const Vector2D& vector) const {
  return this->magnitude() > vector.magnitude();
}
bool Vector2D::operator<=(const Vector2D& vector) const {
  return this->magnitude() <= vector.magnitude();
}
bool Vector2D::operator>=(const Vector2D& vector) const {
  return this->magnitude() >= vector.magnitude();
}

Vector2D Vector2D::operator+(const Vector2D& vector) const { return Vector2D(x + vector.x, y + vector.y); }
Vector2D Vector2D::operator-(const Vector2D& vector) const { return Vector2D(x - vector.x, y - vector.y); }
Vector2D& Vector2D::operator+=(const Vector2D& vector) {
  x += vector.x; y+= vector.y;
  return *this;
}
Vector2D& Vector2D::operator-=(const Vector2D& vector) {
  x -= vector.x; y-= vector.y;
  return *this;
}

Vector2D Vector2D::operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
Vector2D Vector2D::operator/(float scalar) const { return Vector2D(x / scalar, y / scalar); }
Vector2D& Vector2D::operator*=(float scalar) {
  x *= scalar; y *= scalar;
  return *this;
}
Vector2D& Vector2D::operator/=(float scalar) {
  x /= scalar; y /= scalar;
  return *this;
}

Vector2D Vector2D::operator-() const { return Vector2D(-x, -y); }
Vector2D& Vector2D::operator=(const Vector2D& vector) {
  x = vector.x; y = vector.y;
  deg = vector.deg; rad = vector.rad;
  return *this;
}
Vector2D& Vector2D::operator++(int dummy) {
  x++; y++;
  return *this;
}
Vector2D& Vector2D::operator--(int dummy) {
  x--; y--;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vector) {
  os << "(" << vector.x << ", " << vector.y << ")";
  return os;
}

std::istream& operator>>(std::istream& is, Vector2D& vector) {
  is >> vector.x >> vector.y;
  return is;
}

float &Vector2D::operator[](int index) {
  switch (index) {
    case 0: return x;
    case 1: return y;
    default: {
      std::cout << "ERROR: Vector index out of bounds" << std::endl;
      exit(1);
    }
  }
}
