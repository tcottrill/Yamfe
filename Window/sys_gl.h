void set_ortho_view(int width, int height);
void gl_resize(int width, int height); 
int EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
int EnableOpenGL3(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void win_set_vsync(bool enable);