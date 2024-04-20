{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.glfw
    pkgs.mesa
    pkgs.libGL
    pkgs.pkg-config
    pkgs.cglm
    # Add any other dependencies your project may need
  ];

}
