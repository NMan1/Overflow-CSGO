#include "netvars.hpp"
#include "../../interfaces/interfaces.hpp"

//antario if i remember correctly
namespace netvar_manager {
	intptr_t get_offset(recv_table* table, const char* tableName, const char* netvarName)
	{
		for (int i = 0; i < table->props_count; i++)
		{
			recv_prop prop = table->props[i];

			if (!_stricmp(prop.prop_name, netvarName))
			{
				return prop.offset;
			}

			if (prop.data_table)
			{
				intptr_t offset = get_offset(prop.data_table, tableName, netvarName);

				if (offset)
				{
					return offset + prop.offset;
				}
			}
		}
		return 0;
	}

	intptr_t get_netvar_offset(const char* tableName, const char* netvarName)
	{
		c_client_class* currNode = interfaces::client->get_client_classes();

		for (auto currNode = interfaces::client->get_client_classes(); currNode; currNode = currNode->next_ptr)
		{
			if (!_stricmp(tableName, currNode->recvtable_ptr->table_name))
			{
				return get_offset(currNode->recvtable_ptr, tableName, netvarName);
			}
		}

		return 0;
	}
}