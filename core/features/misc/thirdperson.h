#pragma once
#include "../../../dependencies/common_includes.hpp"

static inline void UTIL_TraceHull(vec3_t& vecAbsStart, vec3_t& vecAbsEnd, vec3_t& hullMin,
    vec3_t& hullMax, unsigned int mask, trace_filter* pFilter, trace_t* ptr)
{
    ray_t ray;
    ray.initialize(vecAbsStart, vecAbsEnd, hullMin, hullMax);

    interfaces::trace_ray->trace_ray(ray, mask, pFilter, ptr);
}

static vec3_t GetDesiredCameraOffset()
{
    auto cam_idealdist = interfaces::console->get_convar("cam_idealdist");
    auto cam_idealdistright = interfaces::console->get_convar("cam_idealdistright");
    auto* cam_idealdistup = interfaces::console->get_convar("cam_idealdistup");

    return vec3_t(cam_idealdist->float_value, cam_idealdistright->float_value, cam_idealdistup->float_value);
}

constexpr float    cam_hull_offset{ 16.0f };
     vec3_t   cam_hull_min(-cam_hull_offset, -cam_hull_offset, -cam_hull_offset);
     vec3_t   cam_hull_max(cam_hull_offset, cam_hull_offset, cam_hull_offset);

static void pos_camera(player_t* pPlayer, vec3_t angles)
{ // this is just a modified PositionCamera from the source sdk with stuff we dont need removed & simplified :)
    if (pPlayer) {
        // get player position
        vec3_t origin = pPlayer->origin() + pPlayer->view_offset();
        
        // setup our vectors
        vec3_t camForward, camRight, camUp;
        math::angle_vectors(angles, &camForward, &camRight, &camUp);
        vec3_t endPos = origin;
        vec3_t vecCamOffset = endPos
            + (camForward * -GetDesiredCameraOffset()[0])
            + (camRight * GetDesiredCameraOffset()[1])
            + (camUp * GetDesiredCameraOffset()[2]);

        // filter out player (duh)
        trace_filter traceFilter;
        traceFilter.skip = pPlayer;

        trace_t trace;

        // use UTIL_TraceHull... this is VERY unnecessary, yet makes things slightly easier
        UTIL_TraceHull(endPos, vecCamOffset, cam_hull_min, cam_hull_max, MASK_SOLID & ~CONTENTS_MONSTER, &traceFilter, &trace);

        // if we hit something...
        if (trace.flFraction < 1.0) {
            // move the camera forward
            interfaces::input->m_vecCameraOffset[2] *= trace.flFraction;
        }
    }
}