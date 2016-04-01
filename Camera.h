#include "vectors.h"

class Camera {

public:
	Camera(Vec3 pos, Vec3 heading);
	~Camera();

private:

	Vec3 _pos;
	Vec3 _heading;

};
