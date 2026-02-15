builddir := "./build"

run: build
    ./{{builddir}}/testt

example: build
    ./{{builddir}}/example

build:
    meson compile -C {{builddir}}

setup:
    meson setup {{builddir}} --buildtype=debug -Db_lundef=false -Db_sanitize=address,undefined --reconfigure

setup_release:
    meson setup {{builddir}} --buildtype=release --reconfigure

clean:
    rm -rf ./{{builddir}}
