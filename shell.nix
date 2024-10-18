{pkgs ? import <nixpkgs> {}}:

pkgs.mkShell {
  name = "dev-shell";

  packages = [
    (pkgs.python3.withPackages(ps: [
      ps.pandas
      ps.matplotlib
    ]))
  ];
}
