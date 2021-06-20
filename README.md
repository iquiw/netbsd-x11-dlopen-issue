# X11 dlopen issue without RTLD_GLOBAL

## Overview

When using `dlopen()`, some X11 functions fail to be executed if `RTLD_GLOBAL` flag is not set in `dlopen()` call.
The example program of this repository produces the problem.

```console
$ make
gcc -I/usr/X11R7/include -o xim-dl xim-dl.c

$ ./xim-dl
Trying to open XIM without RTLD_GLOBAL
XSetLocaleModifiers() failed
XOpenIM() failed

Trying to open XIM with RTLD_GLOBAL
XSetLocaleModifiers() succeeded: @im=uim
XOpenIM() succeeded
```

## Background

`alacritty` fails to start on NetBSD by the following error.

```console
$ alacritty
thread 'main' panicked at 'Failed to open input method: PotentialInputMethods {
    xmodifiers: Some(
        PotentialInputMethod {
            name: "@im=uim",
            successful: Some(
                false,
            ),
        },
    ),
    fallbacks: [
        PotentialInputMethod {
            name: "@im=local",
            successful: Some(
                false,
            ),
        },
        PotentialInputMethod {
            name: "@im=",
            successful: Some(
                false,
            ),
        },
    ],
    _xim_servers: Ok(
        [
            "@im=uim",
        ],
    ),
}', /home/iku/.cargo/registry/src/github.com-1ecc6299db9ec823/winit-0.24.0/src/platform_impl/linux/x11/mod.rs:134:17
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
```

### Workaround Fix

`alacritty` depends on `x11-dl` crate, which calls `dlopen()` without `RTLD_GLOBAL`.
The following patch is a workaround fix of the problem.

```diff
--- a/x11-dl/src/link.rs
+++ b/x11-dl/src/link.rs
@@ -87,7 +87,7 @@ impl DynamicLibrary {
         },
       };

-      let handle = libc::dlopen(cname.as_ptr(), libc::RTLD_LAZY);
+      let handle = libc::dlopen(cname.as_ptr(), libc::RTLD_LAZY | libc::RTLD_GLOBAL);

       if handle.is_null() {
         let msg = libc::dlerror();
```
