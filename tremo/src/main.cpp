#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>

void ToggleThread(bool *toggle, const char key)
{
	while (true) {
		if (GetAsyncKeyState(key) & 0x01) {
			*toggle = !(*toggle);

            if (*toggle) {
				std::cout << "INFO: On" << std::endl;
            } else {
				std::cout << "INFO: Off" << std::endl;
            }
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}

void TremoThread(const bool* toggle, const int interval)
{
    INPUT input_mouse_down[1] = {};
    INPUT input_mouse_up[1] = {};

    input_mouse_down[0].type = INPUT_MOUSE;
    input_mouse_down[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN; 

    input_mouse_up[0].type = INPUT_MOUSE;
    input_mouse_up[0].mi.dwFlags = MOUSEEVENTF_LEFTUP; 

    UINT array_size = 1;
    int input_size = sizeof(INPUT);

	UINT sent = 0;

    while (true) {
        if (*toggle) {
            sent = SendInput(array_size, input_mouse_down, input_size);
            if (sent != array_size) {
                std::printf("ALERT: SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
            }

            sent = SendInput(array_size, input_mouse_up, input_size);
            if (sent != array_size) {
                std::printf("ALERT: SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
            }
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(interval));
    }
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "ERROR: Not enough arguments were provided" << std::endl
            << "Usage: tremo <interval> <key>" << std::endl;
        return 0;
    }

    const int interval = atoi(argv[1]);
    if (interval == 0) {
        std::cout << "ERROR: Invalid interval" << std::endl;
    }

    const char key = *argv[2];
    bool toggle = false;

    std::cout << "INFO: To stop the program, press Ctrl+C" << std::endl;

    std::thread toggle_thread(ToggleThread, &toggle, key);
    std::thread tremo_thread(TremoThread, &toggle, interval);

    toggle_thread.join();
    tremo_thread.join();

    return 0;
}