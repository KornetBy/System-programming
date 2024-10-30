#include <iostream>
#include <Windows.h>
using namespace std;

void CreateKey() {
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\MyApp", 0, nullptr, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
        wcout << L"Key created/opened successfully." << endl;

        DWORD data = 123;
        RegSetValueEx(hKey, L"MyValue", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));

        const wchar_t* additionalData = L"AdditionalData";
        RegSetValueEx(hKey, L"AdditionalValue", 0, REG_SZ, reinterpret_cast<const BYTE*>(additionalData), (wcslen(additionalData) + 1) * sizeof(wchar_t));


        RegCloseKey(hKey);

        RegFlushKey(HKEY_CURRENT_USER);
        wcout << L"Changes saved successfully." << endl;
    }
    else {
        wcerr << L"Failed to create/open key." << endl;
    }
}

void ReadKey() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\MyApp", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize = 0;
        DWORD dataType;
        RegQueryValueEx(hKey, L"MyValue", 0, &dataType, nullptr, &dataSize);

        BYTE* data = new BYTE[dataSize];

        if (RegQueryValueEx(hKey, L"MyValue", 0, &dataType, data, &dataSize) == ERROR_SUCCESS) {
            if (dataType == REG_DWORD) {
                DWORD value = *reinterpret_cast<DWORD*>(data);
                wcout << L"MyValue: " << value << std::endl;
            }
            else {
                wcerr << L"Unexpected data type." << std::endl;
            }
        }
        else {
            wcerr << L"Failed to read value." << std::endl;
        }


        delete[] data;
        RegCloseKey(hKey);
    }
    else {
        wcerr << L"Failed to open key." << std::endl;
    }
}

void DeleteKey(const wchar_t* keyPath) {
    if (RegDeleteKey(HKEY_CURRENT_USER, keyPath) == ERROR_SUCCESS) {
        wcout << L"Key deleted successfully." << endl;
    }
    else {
        wcerr << L"Failed to delete key." << endl;
    }
}

void RecursiveSearch(HKEY hKey, const wchar_t* targetKeyName, const std::wstring& currentPath) {
    DWORD subkeyCount;
    DWORD maxSubkeyLength;
    if (RegQueryInfoKey(hKey, nullptr, nullptr, nullptr, &subkeyCount, &maxSubkeyLength, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
        wchar_t* subkeyName = new wchar_t[maxSubkeyLength + 1];

        for (DWORD i = 0; i < subkeyCount; ++i) {
            DWORD subkeyNameLength = maxSubkeyLength + 1;
            if (RegEnumKeyEx(hKey, i, subkeyName, &subkeyNameLength, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
 
                wstring fullPath = currentPath + L"\\" + subkeyName;

                if (wcscmp(subkeyName, targetKeyName) == 0) {
                    wcout << L"Key found: " << fullPath << std::endl;
                    delete[] subkeyName;
                    return;
                }
                else {
                    HKEY hSubKey;
                    if (RegOpenKeyEx(hKey, subkeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                        RecursiveSearch(hSubKey, targetKeyName, fullPath);
                        RegCloseKey(hSubKey);
                    }
                }
            }
        }

        delete[] subkeyName;
    }
}

int main() {
    while (true) {
        wcout << L"1. Create/Open Key\n2. Read Key\n3. Search Key\n4. Delete Key\n5. Exit\n";

        int choice;
        wcin >> choice;

        switch (choice) {
        case 1:
            CreateKey();
            break;
        case 2:
            ReadKey();
            break;
        case 3:
        {
            wcout << L"Enter the key name to search: ";
            wchar_t targetKeyName[256];
            wcin >> targetKeyName;
            RecursiveSearch(HKEY_CURRENT_USER, targetKeyName, L"HKEY_CURRENT_USER");
        }
        break;
        case 4:
        {
            wcout << L"Enter the key path to delete: ";
            wchar_t keyPath[256];
            wcin >> keyPath;
            DeleteKey(keyPath);
        }
        break;
        case 5:
            return 0;
        default:
            wcerr << L"Invalid choice. Try again." << endl;
            break;
        }
    }

    return 0;
}
