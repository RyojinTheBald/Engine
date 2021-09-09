#pragma once

#include <glm/gtx/quaternion.hpp>

namespace Component{
    //NOTE: Entities without orientation must be set to unit quaternion ie. glm::quat(1,0,0,0) in order for subsequent orientations to be applied
    typedef glm::quat Orientation;
}