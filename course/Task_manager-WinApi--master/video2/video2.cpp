#include <dshow.h>
#pragma comment(lib, "strmiids.lib")

int main() {
    CoInitialize(NULL);

    ICreateDevEnum* pDevEnum = NULL;
    IEnumMoniker* pEnum = NULL;

    // Создание объекта для перечисления устройств видеозахвата
    if (CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pDevEnum) == S_OK) {
        // Создание перечислителя устройств
        if (pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0) == S_OK) {
            IMoniker* pMoniker = NULL;

            // Перечисление доступных устройств и вывод их имен
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
                IPropertyBag* pPropBag = NULL;

                if (pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag) == S_OK) {
                    VARIANT var;
                    VariantInit(&var);

                    // Получение имени устройства
                    if (pPropBag->Read(L"FriendlyName", &var, 0) == S_OK) {
                        printf("Device: %ls\n", var.bstrVal);
                        VariantClear(&var);
                    }

                    pPropBag->Release();
                }

                pMoniker->Release();
            }

            pEnum->Release();
        }

        pDevEnum->Release();
    }

    CoUninitialize();
    return 0;
}
