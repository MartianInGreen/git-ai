{ lib
, stdenv
, fetchurl
, dpkg
, curl
, nlohmann_json
}:

stdenv.mkDerivation rec {
  pname = "git-ai";
  version = "0.1.0";

  src = fetchurl {
    url = "https://github.com/MartianInGreen/git-ai/releases/download/v${version}/git-ai_${version}_amd64.deb";
    sha256 = "1smk7ks7vwdfihy46xnjhp6qbcfqkl8hab4cakqj1isfcr5ja20s";
  };

  nativeBuildInputs = [
    dpkg
  ];

  buildInputs = [
    curl
    nlohmann_json
  ];

  unpackPhase = ''
    mkdir -p $out
    dpkg-deb -x $src $out
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp $out/usr/bin/gia $out/bin/
    chmod +x $out/bin/gia
  '';

  meta = with lib; {
    description = "Git AI – AI-powered Git assistant";
    homepage    = "https://github.com/MartianInGreen/git-ai";
    license     = licenses.mit;
    platforms   = platforms.unix;
    maintainers = with maintainers; [ MartianInGreen ];
  };
}
