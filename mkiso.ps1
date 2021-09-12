echo "************************"
echo "     Build ps1-iso"
echo "************************"
echo ""

if ( !(Test-Path build) ) {
    mkdir build
}

echo "[*] Compiling cpe"
ccpsx -O2 -Xm -Xo0x80010000 MAIN.C SRC\*.C -o build\build.cpe

echo "[*] Converting to exe"
cpe2x build\build.cpe

echo "[*] Creating iso"
mkpsxiso cdrom.xml -y -q

echo "[*] Done"