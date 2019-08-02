cd ..
./autogen.sh
./configure LDFLAGS="-L`pwd`/db4/lib/" CPPFLAGS="-I`pwd`/db4/include/" --prefix=`pwd`/depends/i686-w64-mingw32
make
cd src
strip beetokd
strip beetok-cli
strip beetok-tx
cd qt
strip beetok-qt.exe
