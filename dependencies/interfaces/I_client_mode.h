#pragma once
#include "../../source-sdk/classes/entities.hpp"
#include "../../source-sdk/math/vector3d.hpp"

class IPanel;

enum class MotionBlurMode_t
{
	MOTION_BLUR_DISABLE = 1,
	MOTION_BLUR_GAME = 2,
	MOTION_BLUR_SFM = 3
};

class c_view_setup
{
public:
	__int32   x;                  //0x0000 
	__int32   x_old;              //0x0004 
	__int32   y;                  //0x0008 
	__int32   y_old;              //0x000C 
	__int32   width;              //0x0010 
	__int32   width_old;          //0x0014 
	__int32   height;             //0x0018 
	__int32   height_old;         //0x001C 
	char      pad_0x0020[0x90];   //0x0020
	float     fov;                //0x00B0 
	float     viewmodel_fov;      //0x00B4 
	vec3_t    origin;             //0x00B8 
	vec3_t    angles;             //0x00C4 
	char      pad_0x00D0[0x7C];   //0x00D0

};//Size=0x014C

class CBaseViewport;
class CBaseHudWeaponSelection;
typedef unsigned long HCursor;

class i_client_mode
{
public:
	virtual             ~i_client_mode() {}
	virtual int         ClientModeCSNormal(void*) = 0;
	virtual void        Init() = 0;
	virtual void        InitViewport() = 0;
	virtual void        Shutdown() = 0;
	virtual void        Enable() = 0;
	virtual void        Disable() = 0;
	virtual void        Layout() = 0;
	virtual IPanel*		GetViewport() = 0;
	virtual void*		GetViewportAnimationController() = 0;
	virtual void        ProcessInput(bool bActive) = 0;
	virtual bool        ShouldDrawDetailObjects() = 0;
	virtual bool        ShouldDrawEntity(player_t* pEnt) = 0;
	virtual bool        ShouldDrawLocalPlayer(player_t* pPlayer) = 0;
	virtual bool        ShouldDrawParticles() = 0;
	virtual bool        ShouldDrawFog(void) = 0;
	virtual void        OverrideView(c_view_setup* pSetup) = 0; // 19
	virtual int         KeyInput(int down, int keynum, const char* pszCurrentBinding) = 0; // 20
	virtual void        StartMessageMode(int iMessageModeType) = 0; //21
	virtual IPanel* GetMessagePanel() = 0; //22
	virtual void        OverrideMouseInput(float* x, float* y) = 0; //23
	virtual bool        CreateMove(float flSampleFrametime, void* pCmd) = 0; // 24
	virtual void        LevelInit(const char* newmap) = 0;
	virtual void        LevelShutdown(void) = 0;
public:
	void* ptr1;
	void* ptr2;
	CBaseViewport* m_pViewport;
	// Message mode handling
	// All modes share a common chat interface
	HCursor			m_CursorNone;
	CBaseHudWeaponSelection* m_pWeaponSelection;
	int						m_nRootSize[2];
};
