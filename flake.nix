{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    nixpkgs-nci.url = "github:StarGate01/nixpkgs/libnfc-nci";
  };

  outputs = { self, nixpkgs, nixpkgs-nci }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      pkgs-nci = nixpkgs-nci.legacyPackages.x86_64-linux;
    in
    {
      devShell.x86_64-linux =
        pkgs.mkShell {
          shellHook = ''
          '';

          buildInputs = with pkgs; [
            gdb
            gcc
            cmake
            libusb1
            libusb-compat-0_1
            pkg-config
            clang-tools
            pkgs-nci.libnfc-nci
          ];
        };
    };
}
