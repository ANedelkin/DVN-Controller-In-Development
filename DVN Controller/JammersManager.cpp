#include "JammersManager.h"

FT_HANDLE JammersManager::OpenConnection(string serialNumber)
{
	FT_HANDLE device;
	FT_OpenEx((void*)serialNumber.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &device);
	FT_SetBaudRate(device, 9600);
	FT_SetDataCharacteristics(device, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	FT_SetFlowControl(device, FT_FLOW_NONE, 0x11, 0x13);
	FT_SetTimeouts(device, 5000, 0);

	return device;
}

vector<string> JammersManager::GetJammers()
{
	vector<string> jammers;

	DWORD devsCount;
	FT_CreateDeviceInfoList(&devsCount);
	_ft_device_list_info_node* devices = new _ft_device_list_info_node[devsCount];
	FT_GetDeviceInfoList(devices, &devsCount);
	for (DWORD i = 0; i < devsCount; i++) {
		if (devices[i].Type != FT_DEVICE_UNKNOWN && strcmp(devices[i].Description, JAMMER_NAME) == 0) {
			char str[cstrlen(JAMMER_NAME) + 17] = JAMMER_NAME;
			str[strlen(JAMMER_NAME)] = ' ';
			strcat(str, devices[i].SerialNumber);
			jammers.push_back(str);
		}
	}

	delete[] devices;

	return jammers;
}

bool JammersManager::SendLoad(string serialNumber, Load* load)
{
	//FT_HANDLE device = OpenConnection(serialNumber);

	const auto& scenarios = load->GetScenarios();
	
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		for (char j = 0; j < GetBandsCount(); j++)
		{
			char wbuff[14];
			LPDWORD bw = 0;
			sprintf(wbuff, "W%c%02d%04d%04d%c\r", '0' + i, j, scenarios[i].GetFreq(j, 0), scenarios[i].GetFreq(j, 1), scenarios[i].IsActive(j) ? 'Y' : 'N');
			//FT_Write(device, wbuff, 14, bw);
		}
	}
	return true;
}

Load* JammersManager::GetLoad(string serialNumber)
{
	return nullptr;
}
