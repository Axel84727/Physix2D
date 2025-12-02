#pragma once

struct vec2
{
    float x;
    float y;

    vec2() : x(0.0f), y(0.0f) {}
    vec2(float x_val, float y_val) : x(x_val), y(y_val) {}
};

inline vec2 operator+(const vec2 &l, const vec2 &r) { return vec2(l.x + r.x, l.y + r.y); }
inline vec2 operator-(const vec2 &l, const vec2 &r) { return vec2(l.x - r.x, l.y - r.y); }
inline vec2 operator*(float s, const vec2 &v) { return vec2(s * v.x, s * v.y); }
inline vec2 operator*(const vec2 &v, float s) { return vec2(v.x * s, v.y * s); }
inline float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }