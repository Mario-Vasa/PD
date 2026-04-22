#include <windows.h>
#include <fstream>

SERVICE_STATUS g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
void WINAPI ServiceCtrlHandler(DWORD);

// Funcția care scrie mesajul
void WriteLog(const char* msg) {
    CreateDirectory(L"C:\\temp", NULL); // asigură existența folderului
    std::ofstream logFile("C:\\temp\\service_log.txt", std::ios_base::app);
    logFile << msg << std::endl;
}

int main() {
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        { (LPWSTR)L"CppHelloWorldService", ServiceMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(ServiceTable)) {
        return GetLastError();
    }

    return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv) {

    g_StatusHandle = RegisterServiceCtrlHandler(L"CppHelloWorldService", ServiceCtrlHandler);
    if (g_StatusHandle == NULL) return;

    // configurare status
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;

    // serviciul pornește
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    // mesajul cerut
    WriteLog("Hello World!");

    // eveniment pentru oprire
    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // serviciul rulează
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    // așteaptă stop
    WaitForSingleObject(g_ServiceStopEvent, INFINITE);

    // serviciul se oprește
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

void WINAPI ServiceCtrlHandler(DWORD CtrlCode) {
    switch (CtrlCode) {
    case SERVICE_CONTROL_STOP:
        if (g_ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
            SetEvent(g_ServiceStopEvent);
        }
        break;
    default:
        break;
    }
}
