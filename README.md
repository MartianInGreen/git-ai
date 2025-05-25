# Git-AI (gia)

Git-AI is a command-line tool that helps you write better git commit messages using AI. It analyzes your staged changes and generates meaningful commit messages.

## Installation

### Debian/Ubuntu (.deb package)

1. Download the latest release from the [releases page](https://github.com/MartianInGreen/git-ai/releases)
2. Install the package:
```bash
sudo dpkg -i git-ai_version_amd64.deb
```

### Nix

#### Using Flakes

Add Git-AI to your flake.nix:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }: {
    # Add a local package to your system packages
    nixosConfigurations.your-system = nixpkgs.lib.nixosSystem {
      system = "x86_64-linux";
      modules = [
        {
          environment.systemPackages = [
            (pkgs.callPackage ./package.nix { }) # Place the downloaded package.nix for example in packages/ within your .nixos configuration. Rename it to for example `git-ai.nix`.
          ];
        }
      ];
    };
  };
}
```

## Usage

1. Stage your changes:
```bash
git add .
```

2. Run `gia` to generate a commit message:
```bash
gia
```

### Options

- `-h, --help`: Show help message
- `-v, --version`: Show version information
- `-m, --model`: Set the model to use (default: gpt-4.1-mini)
- `-t, -i, --instructions "..."`: Include extra instructions to the model
- `--key "..."`: Set the API key to use
- `--base "..."`: Set the API base URL to use

## Requirements

- Git repository
- Staged changes
- API key for the AI model

## License

MIT License - see LICENSE file for details
