{ lib
, stdenv
, cmake
, pkg-config
, curl
, nlohmann_json
, fetchFromGitHub
}:

stdenv.mkDerivation rec {
  pname = "gia";
  version = "0.1.0";

  src = fetchFromGitHub {
    owner = "MartianInGreen";
    repo = "git-ai";
    rev = "main";
    sha256 = "06cabdb964c45a5b3bc86c7503b5bc7ebcdaef2b";
  };

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    curl
    nlohmann_json
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];

  meta = with lib; {
    description = "Git AI - AI-powered Git assistant";
    homepage = "https://github.com/MartianInGreen/git-ai"; 
    license = licenses.mit;  
    platforms = platforms.unix;
    maintainers = with maintainers; [ MartianInGreen ];
  };
}
