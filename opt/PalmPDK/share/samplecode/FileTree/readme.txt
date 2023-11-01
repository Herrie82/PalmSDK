FILETREE SAMPLE

This is a sample showing a more complex hybrid application that uses 
both the Enyo framework from JavaScript and a PDK plugin.

In this app, a plugin is used to read file listings from /media/internal,
allowing the application to present a custom directory view.  This shows
how to use a plugin to do tasks not possible from JavaScript and how
to dispatch calls to the plugin that will return at a later time.

INTERFACE
---------

The plugin exposes one method to JavaScript:

*** var requestId = plugin.getFiles(directory, pattern);

requestID is an integer provided by the plugin to associate the eventual response
with this call.

directory is a string, the location to scan for files.  The root, "/", is actually mapped to
/media/internal on the device as this plugin will only return results for the user
drive.

pattern is a string and is used to filter the files.  This follows KSH pattern matching,
so you can use * and ? and even braces.  To find all PNG and JPG files, you would scan
for "*.{jpg,png}".  You can also scan down a level of directories by matching with a forward slash.  
To get all JPEG pictures in the folders under /media/internal/DCIM/, you could pass directory as
"/DCIM" and pattern as "*/*.jpg".

The code will call a method set on the plugin called getFilesResult(jsonString).
That method should parse the jsonString that follows this format

    { "requestId" : id, 
      "files" : [ { "type" : "file",
                    "name" : "pathname to filename", 
                    "size": integer, 
                    "date": "YYYY-MM-DD HH:MM:SS" },
                  { "type" : "directory",
                    "name" : "pathname to directory", 
                    "date": "YYYY-MM-DD HH:MM:SS" }, ... 
                ]
    }

SPECIAL TECHNIQUES
------------------
This plugin code uses some of the advanced features of the GNU C Library to avoid 
having to write a lot of memory management code.  Rather than implementing our own
string class, instead we use open_memstream(), a call that creates a new FILE stream
for writing.  When it's closed, it coalesses the written data into a single 
NUL-terminated buffer, suitable for use as a C string.  In this code, it's used
to capture the potentially long JSON responses used to send file and directory listings
back to JavaScript.

For matching filenames, we use the POSIX glob() function to read a directory and return 
the files that match the supplied pattern.  This lets you use the same syntax that you'd use
at the UNIX shell for matching files.  glob() also skips files starting with dot, so unless
you explicitly request in the patter, you won't be able to see hidden dot files.

To avoid tying up the method handling thread with doing directory listings, we instead have 
that thread post a user message to the main SDL thread.  That serializes the requests while
allowing the JavaScript method call to return ASAP.  When that event is handled, we use 
PDL_CallJS to return the data to the JavaScript side.

WINDOWS
-------
From the command window, run 

windows\buildit.cmd -- build the plugin

windows\package-it.cmd -- build plugin and build a full IPK with Mojo
app and plugin for device

You will need to use palm-install to install the filetree IPK file on
the device and then you can run it from the launcher.

MAC OS X
--------
mac\builtit_for_device.sh -- build the plugin

mac\package-it.sh -- build plugin and build a full IPK with Mojo app
and plugin for device

You will need to use palm-install to install the filetree IPK file on
the device and then you can run it from the launcher.
