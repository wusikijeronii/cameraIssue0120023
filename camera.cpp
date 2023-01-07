#include "camera.h"
#include <cmath>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// Project the point in [-1, 1] screen space onto the arcball sphere
static glm::quat screen_to_arcball(const glm::vec2 &p);

ArcballCamera::ArcballCamera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    const glm::vec3 dir = center - eye;
    glm::vec3 z_axis = glm::normalize(dir);
    glm::vec3 x_axis = glm::normalize(glm::cross(z_axis, glm::normalize(up)));
    glm::vec3 y_axis = glm::normalize(glm::cross(x_axis, z_axis));
    x_axis = glm::normalize(glm::cross(z_axis, y_axis));
    mTranslation = glm::translate(glm::vec3(0.0f, 0.0f, -glm::length(dir)));
    mRotation = glm::mat4(glm::transpose(glm::mat3(x_axis, y_axis, -z_axis)));
    glm::extractEulerAngleXYZ(mRotation, rotVec.y, rotVec.x, rotVec.z);

    update_camera();
}

void ArcballCamera::rotate(glm::vec2 prev_mouse, glm::vec2 cur_mouse)
{
    // Clamp mouse positions to stay in NDC
    float angleX = 0.00698132f;
    rotVec.x += rotVec.z < 0.0f ? -angleX : angleX;
    glm::vec3 center{mTranslation[3][0], mTranslation[3][1], 0.0f};
        glm::mat4 rot{1.0f};
    rot = glm::rotate(rot, rotVec.y, glm::vec3(1.0f, 0.0f, 0.0f));
    rot = glm::rotate(rot, rotVec.x, glm::vec3(0.0f, 1.0f, 0.0f));
    rot = glm::rotate(rot, rotVec.z, glm::vec3(0.0f, 0.0f, 1.0f));

    mRotation = glm::translate(glm::vec3(-center)) * rot * glm::translate(glm::vec3(center));
    update_camera();
}

void ArcballCamera::pan(glm::vec2 mouse_delta)
{
    const float zoom_amount = std::abs(mTranslation[3][2]);
    glm::vec4 motion(mouse_delta.x * zoom_amount, mouse_delta.y * zoom_amount, 0.f, 0.f);
    // Find the panning amount in the world space
    motion = inv_camera * motion;

    mTranslation = glm::translate(glm::vec3(motion)) * mTranslation;
    update_camera();
}

void ArcballCamera::zoom(const float zoom_amount)
{
    const glm::vec3 motion(0.f, 0.f, zoom_amount);

    mTranslation = glm::translate(motion) * mTranslation;
    update_camera();
}

const glm::mat4 &ArcballCamera::transform() const
{
    return camera;
}

const glm::mat4 &ArcballCamera::inv_transform() const
{
    return inv_camera;
}

glm::vec3 ArcballCamera::eye() const
{
    return glm::vec3{inv_camera * glm::vec4{0, 0, 0, 1}};
}

glm::vec3 ArcballCamera::dir() const
{
    return glm::normalize(glm::vec3{inv_camera * glm::vec4{0, 0, -1, 0}});
}

glm::vec3 ArcballCamera::up() const
{
    return glm::normalize(glm::vec3{inv_camera * glm::vec4{0, 1, 0, 0}});
}

void ArcballCamera::update_camera()
{
    camera = mTranslation * mRotation;
    inv_camera = glm::inverse(camera);
}

glm::quat screen_to_arcball(const glm::vec2 &p)
{
    const float dist = glm::dot(p, p);
    // If we're on/in the sphere return the point on it
    if (dist <= 1.f) {
        return glm::quat(0.0, p.x, p.y, std::sqrt(1.f - dist));
    } else {
        // otherwise we project the point onto the sphere
        const glm::vec2 proj = glm::normalize(p);
        return glm::quat(0.0, proj.x, proj.y, 0.f);
    }
}
