# SAR
Simple Archive Format.

## sar
```
Usage: sar <filename> [-a author] [-t timestamp] [-lat latitude] [-long longitude]
```

## unsar
```
Usage: unsar <filename> [-m | -o-]
Options:
  -m   Only show metadata of the SAR archive
  -o-   Output to terminal instead of creating a file
```

## building
build the project with `make`. install with `sudo make install`. uninstall with `sudo make uninstall`

to run without installing:
```
make
cd bin
LD_LIBRARY_PATH=. ./sar
```
