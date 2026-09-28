#include <windows.h>
#include <math.h>
#include <stdio.h> 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int nCmdShow) {
    hInst = hInstance;
    const wchar_t szClassName[] = L"GraphVariant2";

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = szClassName;

    if (!RegisterClass(&wc)) return FALSE;

    // АНГЛИЙСКИЙ ТЕКСТ: гарантирует отсутствие иероглифов
    HWND hwnd = CreateWindow(szClassName, L"Lab 2 - Function Graph (Variant 2)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) return FALSE;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int cx, cy;

    switch (msg) {
    case WM_SIZE:
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        SetBkMode(hdc, TRANSPARENT);

        int marginX = cx / 10;
        int marginY = cy / 10;

        int x0 = marginX + 30;
        int y0 = cy - marginY - 30;
        int xMax = cx - marginX;
        int yMax = marginY;

        double x_start = 0.0, x_end = 1.0;
        double y_min = 0.0, y_max = 2.5;

        const int n_points = 200;
        double x_vals[n_points], y_vals[n_points];
        const double PI = 3.141592653589793;

        for (int i = 0; i < n_points; i++) {
            x_vals[i] = x_start + i * (x_end - x_start) / (n_points - 1);
            y_vals[i] = 2.0 * cos(x_vals[i] - PI / 6.0);
        }

        double dx = (xMax - x0) / (x_end - x_start);
        double dy = (y0 - yMax) / (y_max - y_min);

        // ОСИ И РАЗМЕТКА
        HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, axisPen);

        MoveToEx(hdc, x0, y0, NULL);
        LineTo(hdc, xMax + 20, y0);
        TextOutW(hdc, xMax + 25, y0 - 10, L"X", 1);

        MoveToEx(hdc, x0, y0, NULL);
        LineTo(hdc, x0, yMax - 20);
        TextOutW(hdc, x0 - 15, yMax - 25, L"Y", 1);

        for (int i = 0; i <= 5; i++) {
            double val = 0.2 * i;
            int px = x0 + (int)(val * dx);

            MoveToEx(hdc, px, y0 - 5, NULL);
            LineTo(hdc, px, y0 + 5);

            wchar_t buf[16];
            swprintf_s(buf, L"%.1f", val);
            TextOutW(hdc, px - 10, y0 + 10, buf, (int)wcslen(buf));
        }

        for (int i = 0; i <= 5; i++) {
            double val = 0.5 * i;
            int py = y0 - (int)(val * dy);

            MoveToEx(hdc, x0 - 5, py, NULL);
            LineTo(hdc, x0 + 5, py);

            wchar_t buf[16];
            swprintf_s(buf, L"%.1f", val);
            TextOutW(hdc, x0 - 35, py - 8, buf, (int)wcslen(buf));
        }

        SelectObject(hdc, oldPen);
        DeleteObject(axisPen);

        // ГРАФИК
        const wchar_t* title = L"y = 2 * cos(x - PI/6)";
        TextOutW(hdc, cx / 2 - 50, marginY / 2, title, (int)wcslen(title));

        HPEN graphPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        oldPen = (HPEN)SelectObject(hdc, graphPen);

        MoveToEx(hdc, x0 + (int)((x_vals[0] - x_start) * dx), y0 - (int)((y_vals[0] - y_min) * dy), NULL);
        for (int i = 1; i < n_points; i++) {
            int px = x0 + (int)((x_vals[i] - x_start) * dx);
            int py = y0 - (int)((y_vals[i] - y_min) * dy);
            LineTo(hdc, px, py);
        }

        SelectObject(hdc, oldPen);
        DeleteObject(graphPen);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}