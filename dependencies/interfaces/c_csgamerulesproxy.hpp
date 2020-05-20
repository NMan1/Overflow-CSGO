#pragma once
#include "..\utilities\netvars\netvars.hpp"

class c_csgamerulesproxy
{
public:
	int round_count()
	{
		auto rounds = netvar_manager::get_netvar_offset("DT_CSGameRulesProxy", "m_totalRoundsPlayed");
		return *reinterpret_cast<int*>(uintptr_t(this) + rounds);
	}	

	bool is_freeze_period()
	{
		auto freeze_period = netvar_manager::get_netvar_offset("DT_CSGameRulesProxy", "m_bFreezePeriod");
		return *reinterpret_cast<bool*>(uintptr_t(this) + freeze_period);
	}

	bool is_valve_ds()
	{
		auto valve_ds = netvar_manager::get_netvar_offset("DT_CSGameRulesProxy", "m_bIsValveDS");
		return *reinterpret_cast<bool*>(uintptr_t(this) + valve_ds);
	}
};
