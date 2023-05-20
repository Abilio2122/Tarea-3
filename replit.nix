{ pkgs }: {
	deps = [
		pkgs.linuxPackages_latest-libre.perf
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}