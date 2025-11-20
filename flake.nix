{
  description = "game";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/d2ed99647a4b195f0bcc440f76edfa10aeb3b743";
    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    flake-utils.url = "github:numtide/flake-utils/11707dc2f618dd54ca8739b309ec4fc024de578b";
  };
  outputs = { ... }@inputs:
    inputs.flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import inputs.nixpkgs { inherit system; overlays = []; };
        cross-pkgs = pkgs.pkgsCross.mingwW64;
        rust-bin = inputs.rust-overlay.lib.mkRustBin { }
          pkgs.pkgsCross.mingwW64.buildPackages;
        rust-pkg = rust-bin.stable."1.88.0".default.override {
          extensions = [ "rust-src" ];
          targets = [ "x86_64-pc-windows-gnu" ];
        };
        deps = import ./deps.nix { inherit pkgs; };
      in
      {
        formatter = pkgs.nixfmt-classic;
        devShells.default = cross-pkgs.mkShell {
          nativeBuildInputs = [ pkgs.cmake rust-pkg pkgs.pkg-config pkgs.flatbuffers ];
          buildInputs = [ cross-pkgs.windows.pthreads ];
		  shellHook = ''zsh'';
        };
        apps.setup = inputs.flake-utils.lib.mkApp {
          drv = pkgs.writeShellApplication {
            name = "setup";
            runtimeInputs = [ ];
            text = deps.setup_script;
          };
        };
      });
}
