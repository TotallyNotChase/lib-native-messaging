$INSTALL_LOGFILE = "install_location.txt"
$INSTALL_DIR = Get-Content -Path ".\${INSTALL_LOGFILE}"

$LIB_NAME = "strfuncs"

Remove-Item -Path "${INSTALL_DIR}\include\${LIB_NAME}.h"
Remove-Item -Path "${INSTALL_DIR}\lib\${LIB_NAME}.lib"
