# INSTALL_DIR is set to cpkgs in ProgramFiles(x86) unless explicitly set
$INSTALL_DIR = if ($INSTALL_DIR) {$INSTALL_DIR} else { "${env:ProgramFiles(x86)}\cpkgs" }
$INSTALL_LOGFILE = "install_location.txt"

$LIB_NAME = "nativemsg"

# Check if \include and \lib folder exists, if not create them 
if (-Not (Test-Path -Path "${INSTALL_DIR}\include"))
{
    New-Item -Path "${INSTALL_DIR}" -Name "include" -ItemType "directory"
}
if (-Not (Test-Path -Path "${INSTALL_DIR}\lib"))
{
    New-Item -Path "${INSTALL_DIR}" -Name "lib" -ItemType "directory"
}
# Copy the files to the corresponding location
Copy-Item -Path ".\Release\${LIB_NAME}.lib" -Destination "${INSTALL_DIR}\lib\${LIB_NAME}.lib" -Recurse
Copy-Item -Path ".\include\${LIB_NAME}.h" -Destination "${INSTALL_DIR}\include\${LIB_NAME}.h" -Recurse

# Take note of the install directory in a fresh log
if (-Not (Test-Path -Path ".\${INSTALL_LOGFILE}"))
{
    New-Item -Path "." -Name $INSTALL_LOGFILE
}
Set-ItemProperty -Path ".\${INSTALL_LOGFILE}" -name IsReadOnly -value $false
Set-Content -Path ".\${INSTALL_LOGFILE}" -Value $INSTALL_DIR
Set-ItemProperty -Path ".\${INSTALL_LOGFILE}" -name IsReadOnly -value $true
