#include "Camera.h"

Camera::Camera(Vec3 pos, Vec3 at) {
	_pos = pos;
	_at = at;
}

Camera::~Camera() {

}

Vec3 Camera::Position() {
	return _pos;
}

Vec3 Camera::At() {
	return _at;
}