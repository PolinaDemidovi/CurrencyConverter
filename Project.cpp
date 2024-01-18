#include <Windows.h>
#include <string>
#include <stdexcept>
#include "resource.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") //включаем системные стили для элементов управления
// Инициализация переменных для управления окном 
HINSTANCE hInstance;
UINT uMsg;
HWND hAmountEdit, hConvertButton, hResultLabel;
HWND hFromCurrencyComboBox, hToCurrencyComboBox;
//Курсы валют которые использует программа
double exchangeRateUSD = 1.0; 
double exchangeRateEUR = 0.91; 
double exchangeRateUAH = 37.86; 
double exchangeRateBYN = 3.27; 
double exchangeRateRUB = 87.64;
// Обработчик неверных значений
bool ErrorHandler(const wchar_t* value) {
    if (wcslen(value) == 0) {
        MessageBox(nullptr, L"Введите корректное значение!", L"Error", MB_ICONERROR);
        return true;
    }
    try {
        std::stod(value);
    }
    catch (const std::invalid_argument&) {
        MessageBox(nullptr, L"Неверная сумма!", L"Error", MB_ICONERROR);
        return true;
    }
    catch (const std::out_of_range&) {
        MessageBox(nullptr, L"Сумма превышает лимит!", L"Error", MB_ICONERROR);
        return true;
    }
    return false;
}
// Функция конвертации валюты
void ConvertCurrency() {
    wchar_t fromCurrency[256], toCurrency[256], fromValue[256], toValue[256];
    double convertedValue = 0;
    // Получаем значения поля ввода и выпадающих списков
    GetWindowText(hFromCurrencyComboBox, fromCurrency, 20);
    GetWindowText(hToCurrencyComboBox, toCurrency, 20);
    GetWindowText(hAmountEdit, fromValue, sizeof(fromValue) / sizeof(fromValue[0]));
    GetWindowText(hResultLabel, toValue, sizeof(fromValue) / sizeof(fromValue[0]));
    if (ErrorHandler(fromValue) == true) return; // Если ошибка, завершаем функцию
    // Операторы на все возможные случаи
    if (!wcscmp(fromCurrency, L"USD")) {
        if (!wcscmp(toCurrency, L"EUR")) convertedValue = std::stod(fromValue) * exchangeRateEUR;
        if (!wcscmp(toCurrency, L"UAH")) convertedValue = std::stod(fromValue) * exchangeRateUAH;
        if (!wcscmp(toCurrency, L"BYN")) convertedValue = std::stod(fromValue) * exchangeRateBYN;
        if (!wcscmp(toCurrency, L"RUB")) convertedValue = std::stod(fromValue) * exchangeRateRUB;
        if (!wcscmp(toCurrency, L"USD")) convertedValue = std::stod(fromValue) * exchangeRateUSD;
    }
    if (!wcscmp(fromCurrency, L"EUR")) {
        if (!wcscmp(toCurrency, L"EUR")) convertedValue = std::stod(fromValue) * (exchangeRateEUR / exchangeRateEUR);
        if (!wcscmp(toCurrency, L"UAH")) convertedValue = std::stod(fromValue) * (exchangeRateUAH / exchangeRateEUR);
        if (!wcscmp(toCurrency, L"BYN")) convertedValue = std::stod(fromValue) * (exchangeRateBYN / exchangeRateEUR);
        if (!wcscmp(toCurrency, L"RUB")) convertedValue = std::stod(fromValue) * (exchangeRateRUB / exchangeRateEUR);
        if (!wcscmp(toCurrency, L"USD")) convertedValue = std::stod(fromValue) * (exchangeRateUSD / exchangeRateEUR);
    }
    if (!wcscmp(fromCurrency, L"UAH")) {
        if (!wcscmp(toCurrency, L"EUR")) convertedValue = std::stod(fromValue) * (exchangeRateEUR / exchangeRateUAH);
        if (!wcscmp(toCurrency, L"UAH")) convertedValue = std::stod(fromValue) * (exchangeRateUAH / exchangeRateUAH);
        if (!wcscmp(toCurrency, L"BYN")) convertedValue = std::stod(fromValue) * (exchangeRateBYN / exchangeRateUAH);
        if (!wcscmp(toCurrency, L"RUB")) convertedValue = std::stod(fromValue) * (exchangeRateRUB / exchangeRateUAH);
        if (!wcscmp(toCurrency, L"USD")) convertedValue = std::stod(fromValue) * (exchangeRateUSD / exchangeRateUAH);
    }
    if (!wcscmp(fromCurrency, L"BYN")) {
        if (!wcscmp(toCurrency, L"EUR")) convertedValue = std::stod(fromValue) * (exchangeRateEUR / exchangeRateBYN);
        if (!wcscmp(toCurrency, L"UAH")) convertedValue = std::stod(fromValue) * (exchangeRateUAH / exchangeRateBYN);
        if (!wcscmp(toCurrency, L"BYN")) convertedValue = std::stod(fromValue) * (exchangeRateBYN / exchangeRateBYN);
        if (!wcscmp(toCurrency, L"RUB")) convertedValue = std::stod(fromValue) * (exchangeRateRUB / exchangeRateBYN);
        if (!wcscmp(toCurrency, L"USD")) convertedValue = std::stod(fromValue) * (exchangeRateUSD / exchangeRateBYN);
    }
    if (!wcscmp(fromCurrency, L"RUB")) {
        if (!wcscmp(toCurrency, L"EUR")) convertedValue = std::stod(fromValue) * (exchangeRateEUR / exchangeRateRUB);
        if (!wcscmp(toCurrency, L"UAH")) convertedValue = std::stod(fromValue) * (exchangeRateUAH / exchangeRateRUB);
        if (!wcscmp(toCurrency, L"BYN")) convertedValue = std::stod(fromValue) * (exchangeRateBYN / exchangeRateRUB);
        if (!wcscmp(toCurrency, L"RUB")) convertedValue = std::stod(fromValue) * (exchangeRateRUB / exchangeRateRUB);
        if (!wcscmp(toCurrency, L"USD")) convertedValue = std::stod(fromValue) * (exchangeRateUSD / exchangeRateRUB);
    }
    convertedValue = int(convertedValue * 100 + 0.5) / 100.0;
    std::wstring result = std::to_wstring(convertedValue);
    SetWindowText(hResultLabel, result.c_str());
}
// Обработчик сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT myFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    switch (message) {
    // Создание элементов управления при создании окна
    case WM_CREATE:
        hAmountEdit = CreateWindowEx(0, L"EDIT", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
            100, 10, 175, 21, hWnd, nullptr, hInstance, nullptr);
        hFromCurrencyComboBox = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            10, 10, 80, 200, hWnd, nullptr, hInstance, nullptr);
        SendMessage(hFromCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"USD");
        SendMessage(hFromCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"EUR");
        SendMessage(hFromCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"UAH");
        SendMessage(hFromCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"BYN");
        SendMessage(hFromCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"RUB");
        SendMessage(hFromCurrencyComboBox, CB_SETCURSEL, 0, 0);
        hResultLabel = CreateWindowEx(0, L"EDIT", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
            100, 40, 175, 21, hWnd, nullptr, hInstance, nullptr);
        hToCurrencyComboBox = CreateWindow(L"COMBOBOX", L" ", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
            10, 40, 80, 200, hWnd, nullptr, hInstance, nullptr);
        SendMessage(hToCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"USD");
        SendMessage(hToCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"EUR");
        SendMessage(hToCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"UAH");
        SendMessage(hToCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"BYN");
        SendMessage(hToCurrencyComboBox, CB_ADDSTRING, 0, (LPARAM)L"RUB");
        SendMessage(hToCurrencyComboBox, CB_SETCURSEL, 0, 0);
        hConvertButton = CreateWindow(L"BUTTON", L"Конвертировать!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            10, 70, 265, 25, hWnd, reinterpret_cast<HMENU>(1), hInstance, nullptr);
        SendMessage(hConvertButton, WM_SETFONT, WPARAM(myFont), TRUE);
        SendMessage(hToCurrencyComboBox, WM_SETFONT, WPARAM(myFont), TRUE);
        SendMessage(hFromCurrencyComboBox, WM_SETFONT, WPARAM(myFont), TRUE);
        SendMessage(hResultLabel, WM_SETFONT, WPARAM(myFont), TRUE);
        SendMessage(hAmountEdit, WM_SETFONT, WPARAM(myFont), TRUE);
        break;
    // Обработка событий элементов управления
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            ConvertCurrency();
        }
        break;
    // Завершение приложения при закрытии окна
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// Главная функция программы
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Регистрация класса окна
    hInstance = hInst;
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"CurrencyConverterClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
    // На случай ошибки регистрации окна
    if (!RegisterClassEx(&wcex)) {
        MessageBox(nullptr, L"Ошибка регистрации окна!", L"Ошибка!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    // Создание главного окна
    HWND hWnd = CreateWindowEx(0, L"CurrencyConverterClass", L"Конвертер валют", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 140, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        MessageBox(nullptr, L"Не удалось создать окно!", L"Ошибка!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    // Цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}



