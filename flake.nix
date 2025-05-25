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
              mkdir -p lib
              [ ! -d "lib/openai-cpp" ] && git clone https://github.com/olrea/openai-cpp.git lib/openai-cpp
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
              # Load environment variables from .env file if it exists
              if [ -f .env ]; then
                echo "Loading environment variables from .env file..."
                export $(cat .env | grep -v '^#' | xargs)
              fi

              mkdir -p lib
              [ ! -d "lib/openai-cpp" ] && git clone https://github.com/olrea/openai-cpp.git lib/openai-cpp
              chmod +x scripts/run.sh

              exec zsh
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
              # Load environment variables from .env file if it exists
              if [ -f .env ]; then
                echo "Loading environment variables from .env file..."
                export $(cat .env | grep -v '^#' | xargs)
              fi

              echo "⚙️  Debug environment loaded"
              export CMAKE_BUILD_TYPE=Debug
              zsh
            '';
          };
        };
      }
    );
}
