#include "JammersManager.h"
#include "LoadTransferProgressFrame.h"

FT_HANDLE JammersManager::OpenConnection(string serialNumber)
{
	FT_HANDLE device;
	FT_STATUS status;
	status = FT_OpenEx((void*)serialNumber.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &device);
	status = FT_SetBaudRate(device, 9600);
	status = FT_SetDataCharacteristics(device, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	status = FT_SetFlowControl(device, FT_FLOW_NONE, 0x11, 0x13);
	status = FT_SetTimeouts(device, 5000, 0);

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
		if (devices[i].Type != FT_DEVICE_UNKNOWN /*&& strcmp(devices[i].Description, JAMMER_NAME) == 0*/) {
			char str[cstrlen(JAMMER_NAME) + 17] = JAMMER_NAME;
			str[strlen(JAMMER_NAME)] = ' ';
			strcat(str, devices[i].SerialNumber);
			jammers.push_back(str);
		}
	}

	delete[] devices;

	return jammers;
}

bool JammersManager::SendLoad(string serialNumber, Load* load, LoadTransferProgressFrame* frame)
{

	FT_STATUS stat;
	FT_HANDLE device = OpenConnection(serialNumber);
	const auto& scenarios = load->GetScenarios();
	char buff[15];
	unsigned long byteCounter = 0;
	wxStopWatch stopWatch;
	
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		for (char j = 0; j < GetBandsCount(); j++)
		{
			sprintf(buff, "W%c%02d%04d%04d%c\r", '0' + i, j, scenarios[i].GetFreq(j, 0), scenarios[i].GetFreq(j, 1), scenarios[i].IsActive(j) ? 'Y' : 'N');
			stat = FT_Write(device, buff, 14, &byteCounter);

			char bytesRead = 0;
			stopWatch.Start();
			while (true)
			{
				DWORD n = 0;
				stat = FT_GetQueueStatus(device, &n);
				if (n) {
					FT_Read(device, buff + bytesRead, n, &byteCounter);
					bytesRead += n;
				}
				if (n && buff[bytesRead - 1] == '\r') {
					if (buff[0] == 'O')
						break;
					else {
						//Invalid data
						return false;
					}
				}
				else if (stopWatch.Time() > 1000) {
					//Timeout
					FT_Close(device);
					return false;
				}
			}
			stopWatch.Pause();
		}
		frame->Increment();
	}
	FT_Close(device);
	return true;
}

Load* JammersManager::GetLoad(string serialNumber)
{
	FT_HANDLE device = OpenConnection(serialNumber);
	Load* load = new Load();
	char buff[14];
	LPDWORD byteCounter = 0;
	wxStopWatch stopWatch;

	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		for (char j = 0; j < GetBandsCount(); j++)
		{
			sprintf(buff, "R%c%02d\r", '0' + i, j);
			FT_Write(device, buff, 5, byteCounter);
			stopWatch.Start();

			char bytesRead = 0;
			while (true)
			{
				DWORD n = 0;
				FT_STATUS stat = FT_GetQueueStatus(device, &n);
				bytesRead += n;
				FT_Read(device, buff, n, byteCounter);
				if (buff[bytesRead - 1] == '\r') {
					if (buff[13] == 'Y')
						load->GetScenarios()[i].TurnOn(j);
					else
						load->GetScenarios()[i].TurnOff(j);
					buff[13] = '\0';
					int parseResult;
					Validation::TryParse(buff + 8, &parseResult);
					load->GetScenarios()[i].SetFreq(j, 1, parseResult);
					buff[8] = '\0';
					Validation::TryParse(buff + 4, &parseResult);
					load->GetScenarios()[i].SetFreq(j, 0, parseResult);
					break;
				}
				else if (stopWatch.Time() > 15) {
					//Timeout
					break;
				}
			}
			stopWatch.Pause();
		}
	}
}
