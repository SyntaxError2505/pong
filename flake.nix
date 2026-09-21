{
  description = "pong game";

  inputs = {
    nixpkgs.url = "https://channels.nixos.org/nixpkgs-unstable/nixexprs.tar.zst";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }@inputs:
  flake-utils.lib.eachDefaultSystem ( system:
  let
    pkgs = nixpkgs.legacyPackages.${system};
    sdl3-pkg = with pkgs; [
      sdl3
      alsa-lib
      jack2
      pipewire
      libpulseaudio
      libdrm
      mesa
      libGL
      libgbm
      libxcb
      libXdmcp
      libffi
      wayland
      libxkbcommon
      libdecor
      libusb1
      libX11
      libXext
      libXcursor
      libXi
      libXfixes
      libXrandr
      libXScrnSaver
      libXtst
    ];
  in {
    devShells.default = pkgs.mkShell {
      packages = with pkgs; [
        gcc
        gnumake
        pkgconf
        clang-tools
      ] ++ sdl3-pkg;
    };
  });
}
