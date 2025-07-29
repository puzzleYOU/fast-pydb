{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        python313 = pkgs.python313;

        python313Debug = pkgs.python313.overrideAttrs (old: {
          configureFlags = (old.configureFlags or []) ++ [ "--with-pydebug" ];
        });

      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            python313
            python313.pkgs.setuptools
            just
          ];
        };

        packages = {
          testOciImage = pkgs.dockerTools.buildImage {
            fromImage = pkgs.dockerTools.pullImage {
              imageName = "debian";
              imageDigest = "sha256:b6507e340c43553136f5078284c8c68d86ec8262b1724dde73c325e8d3dcdeba";
              sha256 = "EmivyZnwrsIQeLNfcGSS6/zLk3AkfQUPrURrqE0Wjhk=";
            };

            name = "fast-pydb-python313-debug";
            tag = "latest";

            copyToRoot = with pkgs; [
              python313Debug
              python313Packages.setuptools
              valgrind
              gcc
              bash
            ];

            config = {
              Cmd = [ "/bin/bash"];
              WorkingDir = "/code";
              Env = [
                "PYTHONPATH=/lib/python3.13/site-packages:/code"
              ];
            };
          };
        };
      }
  );
}