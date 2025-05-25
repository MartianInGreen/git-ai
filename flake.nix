{
  description = "C++ project with OpenAI integration";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = {
          default = pkgs.stdenv.mkDerivation {
            name = "gia";
            src = ./.;
            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];
            buildInputs = with pkgs; [
              curl
              nlohmann_json
            ];
            buildPhase = ''
              cmake -B build
              cmake --build build
            '';
            installPhase = ''
              mkdir -p $out/bin
              cp build/gia $out/bin/
            '';
          };
        };

        devShells = {
          default = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
              gdb
            ];
            buildInputs = with pkgs; [
              curl
              nlohmann_json
            ];
            shellHook = ''
              echo "Development environment loaded"
            '';
          };

          debug = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
              gdb
              valgrind
            ];
            buildInputs = with pkgs; [
              curl
              nlohmann_json
            ];
            shellHook = ''
              echo "Debug environment loaded"
              export CMAKE_BUILD_TYPE=Debug
            '';
          };
        };
      }
    );
}
