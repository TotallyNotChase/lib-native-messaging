# libnativemsg
A blazing fast, memory safe, [Native Messaging](https://stackoverflow.com/questions/25169384/what-is-native-messaging-between-applications-and-how-does-it-work) host library for C

This facillitates communicating with firefox or chrome extensions from native applications.

Instead of manually reading and writing from `stdin` and `stdout` in your host program, you can simply use 2 simple functions to instantly get started with native messaging in C! With absolutely no sacrifice to performance or resource efficiency!

# Wait, what's Native Messaging?
Simple, a way to establish communication between a browser extension and a native application. Check out the respective docs!
* [Mozilla Firefox](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Native_messaging)
* [Google Chrome](https://developer.chrome.com/extensions/nativeMessaging)

# Installation
* On Linux-

  ## PRE
  Before installing, note that by default the makefile will use `gcc`. You may change this by using `export CC = desired_compiler`. For example you can change the compiler to clang by using `export CC = clang`

  The **install rule** also installs the library and the header file in `/usr/local`. If you'd like to install it globally, use `export INSTALL_DIR = /usr`

  ## Makefile
  Use `$ make build` to **build** the static library, this library is then put inside `build/` with the name `libnativemsg.a`

  Use `$ make install` to **install** this library under `/usr/local/lib` (by default) and also copy over the header file in `include/` to `/usr/local/include` (by default) (**Note**: using `sudo` may be required)
  
  Use `$ make uninstall` to **remove** the library and the header file from `/usr/local` (by default) (**Note**: using `sudo` may be required)

  Use `$ make clean` to **cleanup** the built files in the repo directory, in case something goes wrong
  
* On Windows-
  
  ## PRE
  You'll need **Visual Studio** (2017 or higher) with **clang tools** for building the library

  ## Solution (.sln)
  Open `native-messaging.sln` using your VS and hit `build` in the solution context menu (right click on solution). **Make sure your build config is set to *Release and win32***

  The output `nativemsg.lib` file should be in `Release/`

  ## Script(s)
  Run the powershell script `install.ps1` (in administrator mode) to **install** the package

  By default the install directory is set to `${env:ProgramFiles(x86)}\cpkgs`. If you'd like to install it elsewhere, you can set the variable `$INSTALL_DIR` to your desired path prior to installation. For example, if I'd like to install the package in `Desktop\nativemsg`, I'd do `$INSTALL_DIR = "${env:USERPROFILE}\Desktop\nativemsg"` and then run the script as admin

  Run the powershell script `uninstall.ps1` (in administrator mode) to **uninstall** the package

  During installation, the installation directory is logged inside `install_location.txt`. The uninstall script will read the path from this file.

  **NOTE**: Running powershell scripts **require** you to have *script execution policy set to true*. Read more about it [here](https://stackoverflow.com/questions/10635/why-are-my-powershell-scripts-not-running)

  **NOTE**: You've to run these scripts from the same directory as the scripts themselves

# Test 
If you'd like to test the library prior to installation-
* On Linux-

  Use `$ make test` to build the library and also the test file included in `test_dir/`. This **DOES NOT** install the built library

  You'll notice an output executable file in the current directory named `test`. Link this file to your browser extension.

* On Windows-
  
  Open `test_src\tester.sln` using VS, change build config to **win32** and *preferably* **debug** but **release** works too. Hit `Start` (aka `Build and Run`). This **DOES NOT** install the built library

  **NOTE**: You've to build the actual library (not install) before using `tester.sln`

  You can find docs about how to link the native host [here](https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Native_messaging#App_manifest_2) for firefox and [here](https://developer.chrome.com/extensions/nativeMessaging#native-messaging-host-location) for chrome.

  The test file also relies on **connectionless** messaging.

# API - How to use

## PRE
* On Linux-
  Firstly, you'll need to include the header file-
  ```c
  #include<nativemsg.h>
  ```
  Finally, remember to link the library during compilation. Use the flag `-lnativemsg` for this

* On Windows-
  You'll have to explicitly tell visual studio where to look for header files and you'll also have to link the directory
  
  For the header file, right click on your project and go to properties -> C/C++ -> Additional Include Directories -> Now browse to the location of your `$INSTALL_DIR` and select the `include` folder inside
  
  Now you can include the header file in your source-
  ```c
  #include "nativemsg.h"
  ```
  
  For the library itself, again go to properties -> Linker -> Additional Dependencies -> Now browse to the location of your `$INSTALL_DIR` again and select the `lib` folder inside
  
  Almost done, we now have to link the library itself. To do this you can either include this preprocessor statement in your source file
  
  `#pragma comment(lib, "nativemsg")`
  
  Or you can go to properties -> Linker -> Input -> Additional Dependencies -> Type in `nativemsg.lib`

## `uint8_t* nativemsg_read(uint32_t* length)`
Reads the message sent the by browser extension and returns a uint8_t pointer to it

The pointer should be freed using `free()`

param: length - address of a `uint32_t` variable, to store the length of returned buffer

Returns NULL on failure

The corresponding error is written to stderr

Usage: `uint8_t* msg = nativemsg_read(&len);`

Where `len` is a variable of type `uint32_t`

**NOTE**: The returned pointer does not include a null terminator

## `size_t nativemsg_write(const uint8_t* const buffer, uint32_t length)`
Writes given message for the browser extension to read

@param: buffer - pointer to the array of bytes to be written

@param: length - length of the array - should not be more than (1024 * 1024)

Returns number of bytes written to stdout

The number of bytes written is always `length + 4` on success

The 4 extra bytes being the int variable: `length`

Returns 0 on failure

The corresponding error is written to stderr

Usage: `size_t count = nativemsg_write((uint8_t*)"{\"msg\":\"pong\"}", 14);`

This should return `14 + 4` = `18` on success

**NOTE**: The string `"{\"msg\":\"pong\"}"` is actually 15 bytes, the null terminator in the end **must not be written**. Otherwise it'll crash the extension.

# Examples - Native Host
Assume the sent message is the JSON - `{text: "ping"}`
* ## Connectionless messaging
  ```c++
  uint32_t len;
  // Read the message
  uint8_t* msg = NULL;
  msg = nativemsg_read(&len);
  if (msg == NULL)
  {
    exit(EXIT_FAILURE);
  }
  // Assign a null terminator in the end to make it usable as a string
  msg[len] = '\0';
  if (!strcmp((char*)msg, "{\"text\":\"ping\"}"))
  {
    // Message was 'ping', wrappned inside a json
    // Valid
    free(msg);
    if (nativemsg_write((uint8_t*)"{\"msg\":\"pong\"}", 14) != 18)
    {
        // Wrong number of bytes written - not (14 + 4)
        exit(EXIT_FAILURE);
    }
  }
  ```
* ## Connectionful messaging
  ```c++
  uint32_t len;
  // Read the message
  uint8_t* msg = NULL;
  while(1)
  {
    msg = nativemsg_read(&len);
    if (msg == NULL)
    {
      exit(EXIT_FAILURE);
    }
    // Assign a null terminator in the end to make it usable as a string
    msg[len] = '\0';
    if (!strcmp((char*)msg, "{\"text\":\"ping\"}"))
    {
      // Message was 'ping', wrappned inside a json
      // Valid
      free(msg);
      if (nativemsg_write((uint8_t*)"{\"msg\":\"pong\"}", 14) != 18)
      {
          // Wrong number of bytes written - not (14 + 4)
          exit(EXIT_FAILURE);
      }
    }
  }
  ```

Both of the above examples, expect a json `{text: "ping"}` from the extension and send `{msg: "pong"}` to the extension.

# Examples - Browser Extension
The below examples are meant from **chrome** but there's not a major difference between the firefox version and this one

They both also send the same json mentioned above: `{text: "ping"}` and expect the json: `{msg: "pong"}`

The native host app is named `pingpong`
* ## Sample extension - Connectionless
  ```js
  var start;

  chrome.browserAction.onClicked.addListener(() => {
      console.log('Sending: ping')
      start = performance.now();
      chrome.runtime.sendNativeMessage("pingpong", {text: "ping"}, onResponse);
  });

  function onResponse(res) {
      let end = performance.now();
      console.log(`Received: ${res.msg}, Took: ${end - start} ms`);
  }
  ```
* ## Sample extension - Connectionful
  ```js
  var start;
  var port = chrome.runtime.connectNative('pingpong');

  port.onMessage.addListener(function(res) {
      let end = performance.now();
      console.log(`Received: ${res.msg}, took: ${(end - start) * 1000} μs`);
  });

  port.onDisconnect.addListener(function() {
      console.log("Disconnected");
  });

  chrome.browserAction.onClicked.addListener(() => {
      console.log("Sending: ping")
      start = performance.now();
      port.postMessage({ text: "ping" });
  });
  ```