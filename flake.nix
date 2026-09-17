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
  in {
    devShells.default = pkgs.mkShell {
      packages = with pkgs; [
        gcc
        gnumake
        sdl3
      ];
    };
  });
}
