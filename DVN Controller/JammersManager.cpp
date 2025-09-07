#include "JammersManager.h"
#include "LoadTransferProgressFrame.h"
#include "Log.h"
#include "cstrlen.h"

#define BAND_WRITE_CMD_FORMAT	"W%c%02d%04d%04d%c\r"
#define BAND_WRITE_CMD_LEN		14
#define BAND_WRITE_BUFF_LEN		15

#define BAND_READ_CMD_FORMAT	"R%c%02d\r"
#define BAND_READ_CMD_LEN		5
#define BAND_READ_BUFF_LEN		10

#define BAND_STAT_POS			8
#define BAND_FREQ2_POS			4
#define BAND_FREQ1_POS			0

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

bool JammersManager::SendLoad(string serialNumber, Load* load, function<bool(int, const wxString&)> update)
{

	FT_STATUS stat;
	FT_HANDLE device = OpenConnection(serialNumber);
	const auto& scenarios = load->GetScenarios();
	char buff[BAND_WRITE_BUFF_LEN];
	unsigned long byteCounter = 0;
	wxStopWatch stopWatch;
	//wxStopWatch totalStopWatch;

	//totalStopWatch.Start();
	
	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		for (char j = 0; j < GetBandsCount(); j++)
		{
			sprintf(buff, BAND_WRITE_CMD_FORMAT, '1' + i, j + 1, scenarios[i].GetFreq(j, 0), scenarios[i].GetFreq(j, 1), scenarios[i].IsActive(j) ? 'Y' : 'N');
			stat = FT_Write(device, buff, BAND_WRITE_CMD_LEN, &byteCounter);

			char bytesRead = 0;
			stopWatch.Start();
			while (true)
			{
				update(i, wxString::Format("Scenario: %d/%d", i + 1, SCENARIOS_COUNT));
				DWORD n = 0;
				stat = FT_GetQueueStatus(device, &n);
				if (n) {
					FT_Read(device, buff + bytesRead, n, &byteCounter);
					bytesRead += n;
				}
				if (n && buff[bytesRead - 1] == '\r') {
					if (buff[0] == 'O') {
						//Log(to_string(stopWatch.Time()));
						break;
					}
					else {
						//Invalid data
						FT_Close(device);
						return false;
					}
				}
				else if (stopWatch.Time() > 100) {
					//Timeout
					FT_Close(device);
					return false;
				}
			}
			stopWatch.Pause();
		}
	}
	//totalStopWatch.Pause();
	//int totalEllapsed = totalStopWatch.Time();
	//Log(to_string(totalEllapsed));
	FT_Close(device);
	return true;
}

bool JammersManager::GetLoad(string serialNumber, Load* output, vector<tuple<char, char>>* brokenBands, function<bool(int, const wxString&)> update)
{
	FT_HANDLE device = OpenConnection(serialNumber);
	array<Scenario, SCENARIOS_COUNT>& scenarios = output->GetScenarios();
	char buff[BAND_READ_BUFF_LEN];
	unsigned long byteCounter = 0;
	wxStopWatch stopWatch;

	for (char i = 0; i < SCENARIOS_COUNT; i++)
	{
		for (char j = 0; j < GetBandsCount(); j++)
		{
			bool broken = false;

			sprintf(buff, BAND_READ_CMD_FORMAT, '1' + i, j + 1);
			FT_Write(device, buff, BAND_READ_CMD_LEN, &byteCounter);
			stopWatch.Start();

			char bytesRead = 0;
			while (true)
			{
				update(i, wxString::Format("Scenario: %d/%d", i + 1, SCENARIOS_COUNT));
				DWORD n = 0;
				FT_GetQueueStatus(device, &n);
				if (n) {
					FT_Read(device, buff + bytesRead, n, &byteCounter);
					bytesRead += n;
				}
				if (n && buff[bytesRead - 1] == '\r') {
					if (buff[BAND_STAT_POS] == 'Y')
						scenarios[i].TurnOn(j);
					else
						scenarios[i].TurnOff(j);
					buff[BAND_STAT_POS] = '\0';

					int parseResult;
					
					if (!Validation::TryParse(buff + BAND_FREQ2_POS, &parseResult)) return false;
					scenarios[i].SetFreq(j, 1, parseResult);
					buff[BAND_FREQ2_POS] = '\0';

					if (!Validation::TryParse(buff + BAND_FREQ1_POS, &parseResult)) return false;
					scenarios[i].SetFreq(j, 0, parseResult);

					scenarios[i].SetFreq(j, 1, scenarios[i].GetFreq(j, 1));
					
					break;
				}
				else if (stopWatch.Time() > 100) {
					//Timeout
					FT_Close(device);
					return false;
				}
			}
			stopWatch.Pause();
		}
	}

	FT_Close(device);
	return true;
}
