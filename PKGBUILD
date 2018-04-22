# Maintainer: Antony Kellermann <aokellermann@gmail.com>

pkgname=stag
pkgver=1.0.0
pkgrel=1
pkgdesc="Command line image steganographic injector and extractor."
arch=('x86_64')
url="https://github.com/aokellermann/${pkgname}"
license=('MIT')
provides=("${pkgname}")
conflicts=("${pkgname}")
source=("git://github.com/aokellermann/${pkgname}")
md5sums=('SKIP')

build() {
    cd "$srcdir/$pkgname"
    CPPFLAGS="$CPPFLAGS -O2"
	make
}

package() {
    cd "$srcdir/$pkgname"
    make DESTDIR="'$pkgdir'/usr" install
}