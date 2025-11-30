class world;
#pragma once

class ISystem
{
public:
    virtual void update(world &, float dt) = 0;
    virtual ~ISystem() = default;
};