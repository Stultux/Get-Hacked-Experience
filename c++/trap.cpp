#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>



std::atomic<bool> running(true);
HHOOK keyboardHook = NULL;
HHOOK mouseHook = NULL;


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN) {
            if (pKeyBoard->vkCode == 0x59) { // Taste "Y"
                std::cout << "Y wurde gedrückt. Programm wird beendet..." << std::endl;
                running = false;
                PostQuitMessage(0); // Beendet die Nachrichtenschleife
            }
            else {
                std::cout << pKeyBoard->vkCode << std::endl;
                return 1;
            }
        }
    }
    return 1;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam){
    if(nCode == HC_ACTION){
        return 1;
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}


void playVideo() {
    const std::string videoPath = "..\\Assets\\trap_Video.mp4";
    system(("start /max " + videoPath).c_str());
}

int main() {
    std::cout << "Video startet im Vollbildmodus. Drücke 'y', um zu beenden." << std::endl;

    std::thread videoThread(playVideo);

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);



MSG msg;
    while (running) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        
        if (!running) {
            break;
        }
    }

    UnhookWindowsHookEx(mouseHook);
    UnhookWindowsHookEx(keyboardHook);

    std::cout << "Anwendung beendet." << std::endl;
    return 0;
}