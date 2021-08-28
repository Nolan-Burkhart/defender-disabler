#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <winreg.h>
#include <initguid.h>
#include <GPEdit.h>

/*
* 
* https://admx.help/?Category=Windows_10_2016&Policy=Microsoft.Policies.WindowsDefender
* Source for Registry Keys
* 
* Steps:
* 1. Open Windows Defender Registry.
* 2. Write values to disable it
* 3. Close the handle
* 4. Profit?
*/

bool is_admin() {
	HANDLE token;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
		TOKEN_ELEVATION elevation;
		DWORD size;
		if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
			return elevation.TokenIsElevated;
		}
	}
	return false;
}

int main() {
	if (!is_admin()) {
		std::cout << "[-] run the program as admin. press enter to dismiss.\n";
		system("pause");
		return 0;
	}

	HKEY key;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &key)) {
		std::cout << "[-] failed to open registry.\n";
		system("pause");
		return 0;
	}

	uint32_t payload = 1;
	if (RegSetValueEx(key, "DisableAntiSpyware", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	HKEY new_key;
	if (RegCreateKeyEx(key, "Real-Time Protection", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &new_key, 0)) {
		std::cout << "[-] failed to write to create new key.\n";
		system("pause");
		return 0;
	}
	key = new_key;

	if (RegSetValueEx(key, "DisableRealtimeMonitoring", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	if (RegSetValueEx(key, "DisableBehaviorMonitoring", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	if (RegSetValueEx(key, "DisableOnAccessProtection", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	if (RegSetValueEx(key, "DisableScanOnRealtimeEnable", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	if (RegSetValueEx(key, "DisableIOAVProtection", 0, REG_DWORD, (LPBYTE)&payload, sizeof(payload))) {
		std::cout << "[-] failed to write to registry.\n";
		system("pause");
		return 0;
	}

	RegCloseKey(key);

	std::cout << "[+] registry values written\npress any key to restart to apply them.\n";
	system("pause");
	system("C:\\Windows\\System32\\shutdown /s /t 0");

	return 1;
}