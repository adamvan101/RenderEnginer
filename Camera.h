#include "transformations.h"

class Camera {

public:
	Camera(Vec3 pos, Vec3 at);
	~Camera();

	Vec3 Position();
	Vec3 At();

private:

	Vec3 _pos;
	Vec3 _at;

};
