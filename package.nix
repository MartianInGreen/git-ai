{ lib
, stdenv
, cmake
, pkg-config
, curl
, nlohmann_json
, fetchFromGitHub
}:

let
  openai-cpp = fetchFromGitHub {
    owner  = "olrea";
    repo   = "openai-cpp";
    rev    = "9554a4d86d7650bb3b46db772dbacd5f3e054b8c";
    sha256 = "014avhcn5fnglc7fd8vxmf07yb45h54nrg245siyk6gckngwgcxp";
  };
in

stdenv.mkDerivation rec {
  pname    = "gia";
  version  = "0.1.0";

  src = fetchFromGitHub {
    owner  = "MartianInGreen";
    repo   = "git-ai";
    rev    = "main";
    sha256 = "19yjmb3s4wibpzgx7df9gm0ass8qr1n3pvs9kk4c88j6yvah7h6v";
  };

  nativeBuildInputs = [ cmake pkg-config ];
  buildInputs       = [ curl nlohmann_json openai-cpp ];

  # copy headers into your vendored path
  preConfigure = ''
    mkdir -p lib/openai-cpp/include/openai-cpp
    cp -r ${openai-cpp}/include/* lib/openai-cpp/include/
  '';

  # override the default configure/build/install phases:
  configurePhase = ''
    cmake -S . -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=$out
  '';

  buildPhase = ''
    cmake --build build -- -j${stdenv.jobCount}
  '';

  installPhase = ''
    cmake --install build
  '';

  meta = with lib; {
    description = "Git AI – AI-powered Git assistant";
    homepage    = "https://github.com/MartianInGreen/git-ai";
    license     = licenses.mit;
    platforms   = platforms.unix;
  };
}
