# Maintainer: Ben Morgan <neembi@googlemail.com>
pkgname=libcassava
pkgver=0.1.0
pkgrel=1
pkgdesc="Collection of various C functions created and used by user cassava."
arch=('i686 x86_64')
url="https://github.com/cassava/libcassava"
license=('MIT')
depends=(pacman)
source=(https://github.com/downloads/cassava/$pkgname/$pkgname-$pkgver.tar.gz)

build() {
  cd $srcdir/$pkgname-$pkgver
  make || return 1
}

package() {
  cd $srcdir/$pkgname-$pkgver

  # Install repo program
  make install PREFIX="$pkgdir/usr"
}
