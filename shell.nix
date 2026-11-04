{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "minesweeper-sfml3-env";

  # Khai báo các gói cần thiết
  buildInputs = with pkgs; [
    gcc             # Trình biên dịch C++ (C++17/20)
    gnumake         # Công cụ make (nếu bạn dùng Makefile)
    pkg-config      # Giúp C++ tìm đường dẫn thư viện SFML
    sfml            # Thư viện SFML (Trên nhánh unstable thường mặc định là bản mới nhất)
  ];

  # Lệnh tự động chạy khi bạn gõ nix-shell
  shellHook = ''
  # Sinh compile_flags.txt cho clangd (mỗi cờ một dòng)
  {
    echo "-std=c++17"
    pkg-config --cflags sfml-graphics | tr ' ' '\n'
  } > compile_flags.txt

  echo "============================================="
  echo " Môi trường lập trình SFML đã sẵn sàng!      "
  echo " Trình biên dịch: $(g++ --version | head -n 1)"
  echo "============================================="
'';
}