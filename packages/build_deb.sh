#!/bin/bash

VERSION=$(../release/keyzz/keyzz --version)
PKGDIR=keyzz_${VERSION}_amd64

rm -Rf ${PKGDIR}/
cp -R template/ ${PKGDIR}/

sed -i "s/{{VERSION}}/${VERSION}/" ${PKGDIR}/DEBIAN/control
mkdir ${PKGDIR}/usr/bin/
cp ../release/keyzz/keyzz ${PKGDIR}/usr/bin/

dpkg-deb --root-owner-group --build ${PKGDIR}/
