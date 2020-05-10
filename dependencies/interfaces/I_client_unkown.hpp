#pragma once
#include "I_client_networkable.hpp"
#include "I_client_renderable.hpp"
#include "i_client_thinkable.h"
#include "c_handle.hpp"

class IClientAlphaProperty;
class player_t;
class IClientEntity;


class ICollideable
{
public:
    virtual void pad0();
    virtual const vec3_t& OBBMins() const;
    virtual const vec3_t& OBBMaxs() const;
};

class i_client_renderable;
class i_client_networkable;
class i_client_thinkable;

class i_client_unknown : public IHandleEntity
{
public:
    virtual ICollideable* GetCollideable() = 0;
    virtual i_client_networkable* GetClientNetworkable() = 0;
    virtual i_client_renderable* GetClientRenderable() = 0;
    virtual IClientEntity* GetIClientEntity() = 0;
    virtual player_t* GetBaseEntity() = 0;
    virtual i_client_thinkable* GetClientThinkable() = 0;
    //virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
    virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};