//
// Created by goksu on 2/25/20.
//
#include "Scene.hpp"
#include <functional>
#include <queue>

struct hit_payload
{
    float tNear;
    uint32_t index;
    Vector2f uv;
    Object* hit_obj;
};
struct task_data{
    int m;
    Vector3f eye_pos;
    Vector3f dir;
    int depth;
    int ssp;
    int id;
};
struct task{
    task_data data;
    std::function<void(task_data)> func;
};
class Renderer
{
public:
    void Render(const Scene& scene);

private:
};
