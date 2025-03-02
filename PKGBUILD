pkgname=laptop-server-tools
pkgver=1
pkgrel=1
pkgdesc='Utility for mobile devices'
arch=('x86_64')
url="https://github.com/dr0p-out/laptop_server_tools"
license=('custom')
makedepends=('cmake' 'ninja')

build() {
  cmake -B build -S "${srcdir}/../" -G Ninja -DCMAKE_INSTALL_PREFIX="/usr" -Wno-dev
  cmake --build build
}

package() {
  DESTDIR="$pkgdir" cmake --install build
  install -Dm644 ../supplywatchd.serivce "${pkgdir}/usr/lib/systemd/system/supplywatchd.service"
}
