echo "************************"
echo "     Build ps1-iso"
echo "************************"
echo ""

if ( !(Test-Path build) ) {
    mkdir build
}

echo "[*] Compiling cpe"
ccpsx -O2 -Xm -Xo0x80010000 MAIN.C SRC\GAMEPAD.C SRC\GRAPHICS.C SRC\PLAYER.C SRC\CDROM.C -o build\build.cpe

echo "[*] Converting to exe"
cpe2x build\build.cpe

echo "[*] Creating iso"
mkpsxiso cdrom.xml -y -q

echo "[*] Done"