{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
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

        src = pkgs.lib.cleanSource ./.;

      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            python313
            python313.pkgs.setuptools
            just
          ];
        };

        packages = {
          tests = pkgs.writeShellApplication {
            name = "tests";

            runtimeInputs = with pkgs; [
              python313Debug
              python313.pkgs.setuptools
              valgrind
              gcc
              bash
              src
              busybox
            ];

            text = ''
              #!/bin/sh

              tests/runner.sh
            '';
          };
        };
      }
  );
}
