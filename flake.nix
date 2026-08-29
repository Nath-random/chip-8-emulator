{
  description = "Dev environment for CHIP-8 Emulator in C++ using SDL2";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in
  {
    devShells.${system}.default = pkgs.mkShell {
      packages = [
        pkgs.cmake
        pkgs.clang
        pkgs.pkg-config
        pkgs.SDL2
      ];
    };
  };
}
