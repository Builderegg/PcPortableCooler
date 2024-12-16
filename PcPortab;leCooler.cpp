#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <thread>
#pragma comment(lib, "winmm.lib")
#define MOVE_RIGHT -1
#define MOVE_LEFT 1
void shakeWindow(int moveDirection, HWND hWindow) {
    RECT windowRect;
    if (GetWindowRect(hWindow, &windowRect)) {
        LONG moveAxisX = windowRect.left;
        if (moveDirection == MOVE_RIGHT)
            moveAxisX += 10;
        else if (moveDirection == MOVE_LEFT)
            moveAxisX -= 10;
        SetWindowPos(hWindow, NULL, moveAxisX, windowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
HWND FindTaskManagerWindow() {
    return FindWindow(NULL, L"Gestione attività");
}
void OpenURL(const wchar_t* url) {
    ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!PlaySoundA("sf.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC)) {
        MessageBoxA(NULL, "Failed to play sound. Ensure 'sf.wav' exists in the working directory.", "Error", MB_ICONERROR);
        return 1;
    }
    int currentMoveDirection = MOVE_LEFT;
    bool taskManagerDetected = false;
    while (true) {
        Sleep(35);
        HWND taskManager = FindTaskManagerWindow();
        if (taskManager && !taskManagerDetected) {
            PlaySoundA(NULL, NULL, 0);
            OpenURL(L"https://www.youtube.com/watch?v=TFlku89PE0I");
            Sleep(5000);
            taskManagerDetected = true;
            return 0;
        }
        if (!taskManagerDetected) {
            HWND hWindow = GetForegroundWindow();
            if (hWindow) {
                shakeWindow(currentMoveDirection, hWindow);
            }
            currentMoveDirection = (currentMoveDirection == MOVE_LEFT) ? MOVE_RIGHT : MOVE_LEFT;
        }
    }
    return 0;
}