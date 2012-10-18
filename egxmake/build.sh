#/bin/bash
./egxmake.py >/dev/null

mv out.egx ../sample/egxmake/out.egx
cp ../sample/egxmake/header.egx ../sample/egxmake/test.egx
cat ../sample/egxmake/out.egx >> ../sample/egxmake/test.egx
rm ../sample/egxmake/out.egx
