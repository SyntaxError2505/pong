# Pong
An implementation of pong

## Playing
Get a build of the game for your OS, which is currently only available trough building it yourself.

Rules:
- Ball jumps from upper and lower side of the window.
- Ball jumps between paddles
- Ball respawns if it goes trough left or right side of the window

Keys:
- Up/Down arrows for movement of the right paddle
- W/S keys for movement of the left paddle

## Building it youself
### Dependencies
Dependencies are managed by your package manager, yourself or nix. Required:
- GNU Make
- SDL3
- GCC
- pkg-config
SDL may require other libraries being installed.

### Build process
Build an executable using make:
```bash
make
```
Delete the executable and potential build artifacts using make:
```bash
make clean
```

### Nix
We offer a nix flake that only contains a devshell:
```bash
nix develop
```
After that, the build process stays the same as on any other project.
