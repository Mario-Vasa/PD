#include <windows.h>
#include <setupapi.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "setupapi.lib")

// functie generica pentru citirea unei proprietati
std::wstring GetDeviceProperty(HDEVINFO hDevInfo, SP_DEVINFO_DATA& devData, DWORD property)
{
    DWORD dataType = 0;
    BYTE buffer[1024];
    DWORD size = 0;

    if (SetupDiGetDeviceRegistryPropertyW(
        hDevInfo,
        &devData,
        property,
        &dataType,
        buffer,
        sizeof(buffer),
        &size))
    {
        return std::wstring((wchar_t*)buffer);
    }

    return L"(indisponibil)";
}

// functie care afiseaza mai multe metadate
void PrintDeviceMetadata(HDEVINFO hDevInfo, SP_DEVINFO_DATA& devData, int index)
{
    std::wcout << L"\n=============================\n";
    std::wcout << L"Device #" << index << std::endl;
    std::wcout << L"=============================\n";

    std::wcout << L"Descriere        : " << GetDeviceProperty(hDevInfo, devData, SPDRP_DEVICEDESC) << std::endl;
    std::wcout << L"Friendly Name    : " << GetDeviceProperty(hDevInfo, devData, SPDRP_FRIENDLYNAME) << std::endl;
    std::wcout << L"Producator       : " << GetDeviceProperty(hDevInfo, devData, SPDRP_MFG) << std::endl;
    std::wcout << L"Clasa            : " << GetDeviceProperty(hDevInfo, devData, SPDRP_CLASS) << std::endl;
    std::wcout << L"Class GUID       : " << GetDeviceProperty(hDevInfo, devData, SPDRP_CLASSGUID) << std::endl;
    std::wcout << L"Driver           : " << GetDeviceProperty(hDevInfo, devData, SPDRP_DRIVER) << std::endl;
    std::wcout << L"Hardware ID      : " << GetDeviceProperty(hDevInfo, devData, SPDRP_HARDWAREID) << std::endl;
    std::wcout << L"Location Info    : " << GetDeviceProperty(hDevInfo, devData, SPDRP_LOCATION_INFORMATION) << std::endl;
    std::wcout << L"Enumerator Name  : " << GetDeviceProperty(hDevInfo, devData, SPDRP_ENUMERATOR_NAME) << std::endl;
}

int main()
{
    std::wcout << L"=== Enumerare dispozitive Windows ===\n";

    HDEVINFO hDevInfo = SetupDiGetClassDevsW(
        NULL,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_ALLCLASSES
    );

    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Eroare la obtinerea listei de dispozitive.\n";
        return -1;
    }

    SP_DEVINFO_DATA devData;
    devData.cbSize = sizeof(SP_DEVINFO_DATA);

    int index = 0;

    while (SetupDiEnumDeviceInfo(hDevInfo, index, &devData))
    {
        PrintDeviceMetadata(hDevInfo, devData, index + 1);
        index++;
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);

    std::wcout << L"\nTotal dispozitive detectate: " << index << std::endl;

    return 0;
}
