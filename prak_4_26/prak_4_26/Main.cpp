#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>


#define ID_TIMER_1 1

static TCHAR szMainWindowClass[] = _T("MainWindowClass");
static TCHAR szChildWindowClass[] = _T("ChildWindowClass");

static TCHAR szMainWindowTitle[] = _T("MainWindow");
static TCHAR szChildTitle[] = _T("Child Window");

HINSTANCE hInst;

UINT uFirstDeltaTime = 1000;

BOOL bControlFlag = false;

HWND hWndMain;
HWND hWndChild;

RECT rect;
LONG x = 0;
LONG y = 0;

LONG sizeMain = 600;
LONG sizeChild = 200;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcChild(HWND, UINT, WPARAM, LPARAM);


int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szMainWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    hWndMain = CreateWindow(
        szMainWindowClass,
        szMainWindowTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        sizeMain, sizeMain,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWndMain)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWndMain,
        nCmdShow);
    UpdateWindow(hWndMain);

    SetTimer(hWndMain, ID_TIMER_1, uFirstDeltaTime, NULL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        switch (wParam) {
        case ID_TIMER_1: {

            KillTimer(hWnd, ID_TIMER_1);

            WNDCLASSEX wcexFirstChild;

            wcexFirstChild.cbSize = sizeof(WNDCLASSEX);
            wcexFirstChild.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
            wcexFirstChild.lpfnWndProc = WndProcChild;
            wcexFirstChild.cbClsExtra = 0;
            wcexFirstChild.cbWndExtra = 0;
            wcexFirstChild.hInstance = hInst;
            wcexFirstChild.hIcon = LoadIcon(hInst, IDI_APPLICATION);
            wcexFirstChild.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcexFirstChild.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcexFirstChild.lpszMenuName = NULL;
            wcexFirstChild.lpszClassName = szChildWindowClass;
            wcexFirstChild.hIconSm = LoadIcon(wcexFirstChild.hInstance, IDI_APPLICATION);

            if (!RegisterClassEx(&wcexFirstChild))
            {
                MessageBox(NULL,
                    _T("Call to RegisterClassEx failed! (wcexFirstChild)"),
                    _T("Windows Desktop Guided Tour"),
                    NULL);

                return 1;
            }


            hWndChild = CreateWindow(
                szChildWindowClass,
                szChildTitle,
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                200, 200,
                sizeChild, sizeChild,
                hWnd,
                NULL,
                hInst,
                NULL
            );

            if (!hWndChild)
            {
                MessageBox(NULL,
                    _T("Call to CreateWindow failed! (hWndFirstChild)"),
                    _T("Windows Desktop Guided Tour"),
                    NULL);

                return 1;
            }

            CreateWindow(TEXT("STATIC"), TEXT("1"),
                WS_VISIBLE | WS_CHILD,
                0, 0, 20, 20,
                hWndChild, (HMENU)NULL, NULL, NULL);

           

            break;
        }
        default:
            break;
        }
        break;

    case WM_KEYDOWN:

        switch (wParam) {

        case VK_CONTROL:
            bControlFlag = true;
            break;
        case VK_LEFT: // Обрабатывает клавишу LEFT ARROW (Стрелка влево). 
            GetWindowRect(hWnd, &rect);

            x = rect.left - 20;
            y = rect.top;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeMain, sizeMain, NULL);

            break;

        case VK_RIGHT: // Обрабатывает клавишу RIGHT ARROW (Стрелка вправо). 
            GetWindowRect(hWnd, &rect);

            x = rect.left + 20;
            y = rect.top;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeMain, sizeMain, NULL);
            break;

        case VK_UP: // Обрабатывает клавишу UP ARROW (Стрелка вверх). 
            GetWindowRect(hWnd, &rect);

            x = rect.left;
            y = rect.top - 20;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeMain, sizeMain, NULL);
            break;

        case VK_DOWN: // Обрабатывает клавишу DOWN ARROW (Стрелка вниз). 
            GetWindowRect(hWnd, &rect);

            x = rect.left;
            y = rect.top + 20;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeMain, sizeMain, NULL);
            break;

        case 0x32:      //окно 2

            if (bControlFlag) {

                HWND hWindow = hWndChild; // Дескриптор нужного окна, которому хотим передать фокус
                DWORD dwThreadID = GetWindowThreadProcessId(hWindow, NULL); // Получаем идентификатор потока окна
                DWORD dwThisThreadID = GetWindowThreadProcessId(hWndChild, NULL);
                AttachThreadInput(dwThisThreadID, dwThreadID, TRUE); // Присоединяемся к потоку обработки ввода
                SetFocus(hWindow); // Посылаем сообщение для установки фокуса
                AttachThreadInput(dwThisThreadID, dwThreadID, FALSE); // Отсоединяемся

            }

            break;

        default:
            break;
        }

        break;

    case WM_KEYUP:
        if (wParam == VK_CONTROL) {
            bControlFlag = false;
            MessageBox(NULL,
                _T("CTRL down"),
                _T("MSG"),
                NULL);
        }
        break;
    case WM_LBUTTONDBLCLK:
        GetWindowRect(hWnd, &rect);

        x = rect.left;
        y = rect.top + 20;

        sizeMain = 1000;

        SetWindowPos(hWnd, HWND_TOP, x, y, sizeMain, sizeMain, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}




LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:

        switch (wParam) {

        case VK_CONTROL:
            bControlFlag = true;
            break;
        case VK_LEFT: // Обрабатывает клавишу LEFT ARROW (Стрелка влево). 
            GetWindowRect(hWnd, &rect);

            x = rect.left - 20;
            y = rect.top;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeChild, sizeChild, NULL);

            break;

        case VK_RIGHT: // Обрабатывает клавишу RIGHT ARROW (Стрелка вправо). 
            GetWindowRect(hWnd, &rect);

            x = rect.left + 20;
            y = rect.top;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeChild, sizeChild, NULL);
            break;

        case VK_UP: // Обрабатывает клавишу UP ARROW (Стрелка вверх). 
            GetWindowRect(hWnd, &rect);

            x = rect.left;
            y = rect.top - 20;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeChild, sizeChild, NULL);
            break;

        case VK_DOWN: // Обрабатывает клавишу DOWN ARROW (Стрелка вниз). 
            GetWindowRect(hWnd, &rect);

            x = rect.left;
            y = rect.top + 20;

            SetWindowPos(hWnd, HWND_TOP, x, y, sizeChild, sizeChild, NULL);
            break;

        case 0x31:      //окно 1

            if (bControlFlag) {

                HWND hWindow = hWndMain; // Дескриптор нужного окна, которому хотим передать фокус
                DWORD dwThreadID = GetWindowThreadProcessId(hWindow, NULL); // Получаем идентификатор потока окна
                DWORD dwThisThreadID = GetWindowThreadProcessId(hWndMain, NULL);
                AttachThreadInput(dwThisThreadID, dwThreadID, TRUE); // Присоединяемся к потоку обработки ввода
                SetFocus(hWindow); // Посылаем сообщение для установки фокуса
                AttachThreadInput(dwThisThreadID, dwThreadID, FALSE); // Отсоединяемся

            }

            break;

        default:
            break;
        }

        break;

    case WM_KEYUP:
        if (wParam == VK_CONTROL) {
            bControlFlag = false;
            MessageBox(NULL,
                _T("CTRL down"),
                _T("MSG"),
                NULL);
        }
        break;
    case WM_LBUTTONDBLCLK:
        GetWindowRect(hWnd, &rect);

        x = rect.left;
        y = rect.top + 20;

        sizeChild = 1000;

        SetWindowPos(hWnd, HWND_TOP, x, y, sizeChild, sizeChild, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}