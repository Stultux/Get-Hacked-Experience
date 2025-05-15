#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>


std::atomic<bool> running(true);
HHOOK keyboardHook = NULL;
HHOOK mouseHook = NULL;

// Low-Level Keyboard Hook Callback
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
                // Blockiere alle anderen Tasten global
                std::cout << pKeyBoard->vkCode << std::endl;
                return 1;
            }
        }
    }
    // Weiterleitung der Nachricht verhindern
    return 1;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam){
    if(nCode == HC_ACTION){
        //Blockiert alle Maus inputs
        return 1;
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

// Funktion zum Abspielen des Videos im Vollbildmodus
void playVideo() {
    const std::string videoPath = "..\\Assets\\Video.mp4";
    std::cout << "Video wird gestartet..." << std::endl;
    system(("start /max " + videoPath).c_str());
}

int main() {
    std::cout << "Video startet im Vollbildmodus. Drücke 'y', um zu beenden." << std::endl;

    // Starte das Video in einem eigenen Thread
    std::thread videoThread(playVideo);

    // Installiere den globalen Low-Level-Hook
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    // Nachrichtenschleife starten
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!running) {
            break;
        }
    }

    // Entferne den Hook beim Beenden
    UnhookWindowsHookEx(mouseHook);
    UnhookWindowsHookEx(keyboardHook);

    // Warte auf das Ende des Video-Threads
    if (videoThread.joinable()) {
        videoThread.join();
    }

    std::cout << "Anwendung beendet." << std::endl;
    return 0;
}