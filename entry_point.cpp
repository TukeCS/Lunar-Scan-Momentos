#include <Windows.h>
#include <iostream>
#include <vector>

/* _______________________________
  |                               |
  |     OMG AMOGOS BY TUKE        |
  |  because it be working lunar  |
  |_______________________________| */

using L_U64 = uint64_t;

template<class T>
bool scanner(HANDLE pHandle, T tValue, std::vector<L_U64>& vecAddressList) {

    vecAddressList.clear();

    MEMORY_BASIC_INFORMATION mbi;

    DWORD dwFlags = (PAGE_NOACCESS | PAGE_GUARD | PAGE_NOCACHE);

    for (L_U64 ui64BaseAddress = 0; VirtualQueryEx(pHandle, (LPCVOID)ui64BaseAddress, &mbi, sizeof(mbi)); ui64BaseAddress += mbi.RegionSize) {

        if ((mbi.State & MEM_COMMIT) && !(mbi.Protect & dwFlags) && (mbi.Protect & PAGE_EXECUTE_READWRITE) && ui64BaseAddress != 0) {

            std::vector<T> vecBuffer(mbi.RegionSize);

            if (ReadProcessMemory(pHandle, (LPCVOID)ui64BaseAddress, &vecBuffer[0], vecBuffer.size(), nullptr)) {

                for (L_U64 index = 0; index < vecBuffer.size(); index++) {

                    if (tValue == vecBuffer[index]) {

                        vecAddressList.push_back(ui64BaseAddress + index * sizeof(T));
                    }
                }
            }
        }
    }

    return vecAddressList.size();
}

int main() {

    float flReach = 3.0f; // idk ignore the name its just an example

    HWND hWnd = FindWindowA("LWJGL", nullptr); //minecraft
    DWORD pID = 0;
    GetWindowThreadProcessId(hWnd, &pID);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

    std::vector<L_U64> list;

    if (scanner(pHandle, flReach, list)) {
        std::cout << "Something found" << std::endl;

        for (const auto& i : list) {
            std::cout << std::hex << i << std::endl;
        }
    }
    else
        std::cout << "Nothing found";
  
  return 0;
}
