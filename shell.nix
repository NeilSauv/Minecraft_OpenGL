{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.glfw
    pkgs.mesa
    pkgs.libGL
    pkgs.pkg-config
    pkgs.cglm
    pkgs.python3.pkgs.pillow
  ];

}
