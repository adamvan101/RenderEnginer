#pragma once
#include <stdio.h>
#include <math.h>

class Mat4;

class Vec3{
  public:
    Vec3();
    Vec3(float x, float y, float z);
    float v[3];

    //functions
    void print();
    Vec3 cross(Vec3 rhs);
    float dot(Vec3 rhs);
    float mag();
    Vec3 normal();
    
    Vec3 operator+(const Vec3& rhs) const;
    Vec3 operator-(const Vec3& rhs) const;
    float &operator[](int i);

    static Vec3 Up();
};

class Vec4{
  //NOTE: assuming all of these are column vectors
  public:
    Vec4();
    Vec4(float x, float y, float z, float w);
    float v[4];

    //functions
    Vec3 xyz();
    void print();
    Vec4& operator*=(const Mat4& rhs);
    float &operator[](int i);
};

class Mat4 {
  //NOTE: column major
  public:
    Mat4();
    Mat4(float m[16]);

    float matrix[16];
    
    //functions
    static Mat4 Identity();
    void setVal(int i, float x);
    void print();
    Mat4 operator*(const Mat4& rhs) const;
    Mat4& operator*=(const Mat4& rhs);
    Vec4 operator*(const Vec4& rhs) const;

    //virtual ~Mat4();
};
