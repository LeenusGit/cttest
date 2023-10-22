CXX=g++-12 meson setup builddir/gcc -Db_coverage=true -Dcpp_std=c++23 --wipe
meson setup builddir/clang++-17 --native-file buildconf/clang-17.ini --wipe
