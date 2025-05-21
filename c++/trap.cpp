#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>



std::atomic<bool> running(true);
HANDLE videoProcessHandle = NULL;
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
    const std::string videoPath = "C:\\Users\\V.Huwa\\Desktop\\test\\LF4-Project\\Assets\\trap_Video.mp4";
    system(("start /max " + videoPath).c_str());
}

int main() {
    std::cout << "Video startet im Vollbildmodus. Drücke 'y', um zu beenden." << std::endl;

    // Starte das Video in einem eigenen Thread
    std::thread videoThread(playVideo);

    // Installiere den globalen Low-Level-Hook
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);



MSG msg;
    while (running) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Überprüfen Sie hier, ob Sie das Programm beenden möchten
            // Zum Beispiel nach einer bestimmten Bedingung oder Benutzeraktion
            
            // Wenn Sie möchten, dass das Programm nach einer bestimmten Zeit endet:
            // std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Kurze Pause, um CPU-Last zu reduzieren
        }
        
        // Wenn Sie das Programm beenden möchten:
        if (!running) {
            std::cout << "Bla3" << std::endl;
            
            // Video-Prozess beenden, falls er noch läuft
            if (videoProcessHandle != NULL) {
                TerminateProcess(videoProcessHandle, 0);
                CloseHandle(videoProcessHandle);
                videoProcessHandle = NULL;
            }
            
            std::cout << "Bla4" << std::endl;
            break;
        }
    }
    // MSG msg;
    // while (GetMessage(&msg, NULL, 0, 0) > 0) {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);

    //     std::cout << "Video AHHHHHHH..." << std::endl;
    //     if (!running) {
    //         break;
    //     }
    // }

    // Nachrichtenschleife starten
//     MSG msg;
//     while (running) {
//     if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//         if (msg.message == WM_QUIT) {
//             break;
//         }
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     } else {
//         // Dieser Code wird ausgeführt, wenn keine Nachrichten vorhanden sind

//     }
    
//     if (!running) {
//         break;
//     }
// }


    // Entferne den Hook beim Beenden
    UnhookWindowsHookEx(mouseHook);
    UnhookWindowsHookEx(keyboardHook);

    std::cout << "Anwendung beendet." << std::endl;
    return 0;
}