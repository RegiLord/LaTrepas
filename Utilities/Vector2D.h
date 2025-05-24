//
// Created by Leustean Stefan on 19.03.2025.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <raylib.h>

class Vector2D {
  private:
    float x, y;
    float deg;
    double rad;
  public:
    Vector2D();
    Vector2D(float, float);
    Vector2D(const Vector2D&);
    ~Vector2D();

    float getDeg() const;
    double getRad() const;
    void changeOrientationDeg(float deg);
    void changeOrientationRad(double rad);

    float getX() const;
    float getY() const;
    void setX(float);
    void setY(float);
    void setXY(float, float);
    float magnitude() const;

    bool operator==(const Vector2D&) const;
    bool operator<(const Vector2D&) const;
    bool operator>(const Vector2D&) const;
    bool operator<=(const Vector2D&) const;
    bool operator>=(const Vector2D&) const;
    bool operator!=(const Vector2D&) const;

    Vector2D operator+(const Vector2D&) const;
    Vector2D operator-(const Vector2D&) const;
    Vector2D& operator+=(const Vector2D&);
    Vector2D& operator-=(const Vector2D&);

    Vector2D operator*(float) const;
    Vector2D operator/(float) const;
    Vector2D& operator*=(float);
    Vector2D& operator/=(float);

    Vector2D operator-() const;
    Vector2D& operator=(const Vector2D&);
    Vector2D& operator++(int);
    Vector2D& operator--(int);

    float& operator[](int);
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vector);
    friend std::istream& operator>>(std::istream& is, Vector2D& vector);

    operator Vector2() const {
      return Vector2(this->x, this->y);
    }
};

#endif //VECTOR2D_H
