#pragma once
#include "i_material_system.hpp"
#include "I_client_unkown.hpp"

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;


struct model_t;
class i_client_renderable;

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

struct renderable_instance_t
{
    uint8 m_nAlpha;
};

class i_client_renderable
{
public:
    virtual i_client_unknown* GetIClientUnknown() = 0;
    virtual vec3_t const& GetRenderOrigin(void) = 0;
    virtual vec3_t const& GetRenderAngles(void) = 0;
    virtual bool                       ShouldDraw(void) = 0;
    virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
    virtual void                       Unused(void) const {}
    virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
    virtual ClientRenderHandle_t& RenderHandle() = 0;
    virtual model_t* GetModel() const = 0;
    virtual int                        DrawModel(int flags, const renderable_instance_t& instance) = 0;
    virtual int                        GetBody() = 0;
    virtual void                       GetColorModulation(float* color) = 0;
    virtual bool                       LODTest() = 0;
    virtual bool                       SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
    virtual void                       SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
    virtual void                       DoAnimationEvents(void) = 0;
    virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
    virtual void                       GetRenderBounds(vec3_t& mins, vec3_t& maxs) = 0;
    virtual void                       GetRenderBoundsWorldspace(vec3_t& mins, vec3_t& maxs) = 0;
    virtual void                       GetShadowRenderBounds(vec3_t& mins, vec3_t& maxs, int /*ShadowType_t*/ shadowType) = 0;
    virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
    virtual bool                       GetShadowCastDistance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool                       GetShadowCastDirection(vec3_t* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool                       IsShadowDirty() = 0;
    virtual void                       MarkShadowDirty(bool bDirty) = 0;
    virtual i_client_renderable* GetShadowParent() = 0;
    virtual i_client_renderable* FirstShadowChild() = 0;
    virtual i_client_renderable* NextShadowPeer() = 0;
    virtual int /*ShadowType_t*/       ShadowCastType() = 0;
    virtual void                       CreateModelInstance() = 0;
    virtual ModelInstanceHandle_t      GetModelInstance() = 0;
    virtual const matrix3x4_t& RenderableToWorldTransform() = 0;
    virtual int                        LookupAttachment(const char* pAttachmentName) = 0;
    virtual   bool                     GetAttachment(int number, vec3_t& origin, vec3_t& angles) = 0;
    virtual bool                       GetAttachment(int number, matrix3x4_t& matrix) = 0;
    virtual float* GetRenderClipPlane(void) = 0;
    virtual int                        GetSkin() = 0;
    virtual void                       OnThreadedDrawSetup() = 0;
    virtual bool                       UsesFlexDelayedWeights() = 0;
    virtual void                       RecordToolMessage() = 0;
    virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
    virtual uint8                      OverrideAlphaModulation(uint8 nAlpha) = 0;
    virtual uint8                      OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};

// ik this is really meant to be here but boo fucking hoo
struct RenderableInfo_t {
    i_client_renderable* renderable;
    void* alpha_property;
    int enum_count;
    int render_frame;
    unsigned short first_shadow;
    unsigned short leaf_list;
    short area;
    std::uint16_t flags;
    std::uint16_t flags2;
    vec3_t bloated_abs_mins;
    vec3_t bloated_abs_maxs;
    vec3_t abs_mins;
    vec3_t abs_maxs;
    int pad;
};