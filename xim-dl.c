#include <locale.h>
#include <stdio.h>
#include <dlfcn.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>

void dlopen_openim(int mode) {
    void *handle = dlopen("libX11.so", mode);
    Display *(*xOpenDisplay)(char *) = (Display *(*)(char *)) dlsym(handle, "XOpenDisplay");
    int (*xCloseDisplay)(Display *) = (int (*)(Display *)) dlsym(handle, "XCloseDisplay");
    char *(*xSetLocaleModifiers)(const char *) = (char *(*)(const char *)) dlsym(handle, "XSetLocaleModifiers");
    XIM (*xOpenIM)(Display *, XrmDatabase, char *, char *) = (XIM (*)(Display *, XrmDatabase, char *, char *)) dlsym(handle, "XOpenIM");
    Status (*xCloseIM)(XIM) = (Status (*)(XIM)) dlsym(handle, "XCloseIM");

    Display *disp = (*xOpenDisplay)("");
    if (disp == NULL) {
        return;
    }
    char *modifiers = (*xSetLocaleModifiers)("");
    if (modifiers == NULL) {
        printf("XSetLocaleModifiers() failed\n");
    } else {
        printf("XSetLocaleModifiers() succeeded: %s\n", modifiers);
    }
    XIM xim = (*xOpenIM)(disp, NULL, NULL, NULL);
    if (xim == NULL) {
        printf("XOpenIM() failed\n");
    } else {
        printf("XOpenIM() succeeded\n");
        xCloseIM(xim);
    }
    xCloseDisplay(disp);
    dlclose(handle);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    printf("Trying to open XIM without RTLD_GLOBAL\n");
    dlopen_openim(RTLD_NOW);
    printf("\n");
    printf("Trying to open XIM with RTLD_GLOBAL\n");
    dlopen_openim(RTLD_NOW | RTLD_GLOBAL);
    return 0;
}
