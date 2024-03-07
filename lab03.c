#include <windows.h>
#define ID_COPY 1000
#define ID_QUIT 1001
#define ID_CUT 1002
#define ID_EDIT 1003
#define ID_PASTE 1004
#define ID_OPEN 1005
#define ID_SAVE 1006

const char g_szClassName[] = "myWindowClass";
HWND edit;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            edit = CreateWindowW(L"Edit", L"",
                                 WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                                 15, 15, 300, 200, hwnd, (HMENU)ID_EDIT, NULL, NULL);
            CreateWindowW(L"Button", L"Copy", WS_VISIBLE | WS_CHILD, 20, 250, 80, 25, hwnd,
                          (HMENU)ID_COPY, NULL, NULL);
            CreateWindowW(L"Button", L"Cut", WS_VISIBLE | WS_CHILD, 120, 250, 80, 25, hwnd,
                          (HMENU)ID_CUT, NULL, NULL);
            CreateWindowW(L"Button", L"Quit", WS_VISIBLE | WS_CHILD, 120, 300, 80, 25, hwnd,
                          (HMENU)ID_QUIT, NULL, NULL);
            CreateWindowW(L"Button", L"Paste", WS_VISIBLE | WS_CHILD, 20, 300, 80, 25, hwnd,
                          (HMENU)ID_PASTE, NULL, NULL);
            CreateWindowW(L"Button", L"Open", WS_VISIBLE | WS_CHILD, 220, 250, 80, 25, hwnd,
                          (HMENU)ID_OPEN, NULL, NULL);
            CreateWindowW(L"Button", L"Save", WS_VISIBLE | WS_CHILD, 220, 300, 80, 25, hwnd,
                          (HMENU)ID_SAVE, NULL, NULL);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case ID_COPY:
                    char buff1[1024];
                    DWORD dwStart = 0, dwEnd = 0;
                    char ret;
                    //SendMessage(ret, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
                    GetWindowText(edit, buff1, 1024);
                    ClipboardCopy(buff1); 
                    break;
                case ID_PASTE:
                    char *str = NULL;
                    ClipboardPaste(&str);
                    char buff2[2048];
                    GetWindowText(edit, buff2, 2048);
                    SendMessage(edit, WM_SETTEXT, NULL, strcat(buff2, str));
                    break;
                case ID_CUT:
                    char buff3[2048];
                    GetWindowText(edit, buff3, 2048);
                    memset(buff3,0,2048);
                    SendMessage(edit, WM_SETTEXT, NULL, buff3);
                    break;
                case ID_OPEN:
                    HANDLE FileHandle;
                    FileHandle=CreateFile("file1.txt",GENERIC_READ |
                    GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
                    CloseHandle(FileHandle);




                    //SendMessage(edit, WM_SETTEXT, NULL, Buf);
                case ID_QUIT:
                    PostQuitMessage(0);
                    break;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void ClipboardPaste(char **text)
{
    if (IsClipboardFormatAvailable(CF_TEXT))
    {
        if (OpenClipboard(0))
        {
            char *hBuff = GetClipboardData(CF_TEXT);
            int len = strlen(hBuff)+1;
            *text = realloc(*text, len);
            sprintf(*text, "%s", hBuff);
            
            CloseClipboard();
        }
    }
}

void ClipboardCopy(char *text)
{
    if (OpenClipboard(0))
    {
        EmptyClipboard();
        char *hBuff = GlobalAlloc(GMEM_FIXED, strlen(text)+1);
        strcpy(hBuff, text);
        SetClipboardData(CF_TEXT, hBuff);
        CloseClipboard();
    }
}










int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    HWND hwnd;
    hwnd =
        CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Samiy lychshiy textbook!", WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT, CW_USEDEFAULT, 350, 375, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
