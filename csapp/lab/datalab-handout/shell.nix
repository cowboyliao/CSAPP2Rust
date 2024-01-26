# with (import <nixpkgs> {}).pkgsi686Linux; 
with import <nixpkgs> {}; 
mkShell {
  NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [
  ];
  NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
  buildInputs = [ pkgs.pkgsi686Linux.glibc]; 
  # buildInputs = [ pkgs.glibc ]; 
}
