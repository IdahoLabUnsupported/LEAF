RomeDLL Readme 2007-09-02

The RomeDLL contains a subset of the Rusle2 application:
- no Windows are shown.
- no support for sending bug reports
- no OLE Automation API.

The RomeDLL is a work in progress, especially its documentation.
Comments and requests for enhancement are welcome.

There is a C++ sample RomeRemote which demonstrates using the RomeDLL from a 
command-line MFC application.

By default, the RomeDLL expects to run with the same directory layout as the 
Rusle2 application:

<rootdir>\
  Binaries\
    RomeDLL.DLL
  Users\
    <user-template>.xml       (optional - controls units and variants used)
    <access-file>.acc         (required)
	moses.prm             (required)
  Session\                    (created at runtime)
    commands.xml              (created at runtime)
    errors.log                (created at runtime)
    messages.log              (created at runtime)
  Text\
    en\
      titles_core.txt         (optional - text file for user-interface strings)
      titles_core_*.txt       (optional - text file for user-interface strings)
      titles_model.txt        (optional - text file for user-interface strings)
      titles_model_*.txt      (optional - text file for user-interface strings)
      titles_user.txt         (optional - text file for user-interface strings)
      <access-name>.txt       (optional - titles associated with specific access level[s])
  moses.gdb                   (optional - may be located elsewhere and renamed)


However, this makes it awkward to use the DLL in another application. The directory 
structure can be modified by passing "command line" arguments to the RomeInit() 
API function. The /DirRoot switch causes the DLL to use a specific directory as 
the "root" for its files, instead of its parent directory. The /Path swich allows
redirecting individual directories and files used by Rusle2.
See the RomeInit() documentation for more information.


The database "moses.gdb" is not automatically opened on startup. You must explicitly 
call RomeDatabaseOpen() to connect to a database file, which may be located anywhere, 
and have any filename, although the database file should have extension ".GDB" (case 
insensitive) to be correctly recognized.


The *.TXT files are used to generate human-readable strings for messages generated 
by the Rome DLL, and are used in logging. Note that all of the translation text 
files may be concatenated into a single text file with any of the possible names 
(e.g. "titles_core.txt"), in order to reduce the number of files shipped. 
Note that the RomeDLL will run without ANY text files present.
See developer document "Rusle2 Translation Titles.rtf" for more information.


User commands, messages, and internal logging info is saved in the file 
"Session\commands.xml", which is useful for debugging.


The Rome API functions now all return documented error values, and RomeGetLastError()
can be used to get additional error information right after a failure.


The datadll.DLL code is now compiled into the Rome DLL itself. It is not necessary 
to ship any other DLLs with the RomeDLL.


The structure of the RUSLE2 scientific model is described in XML files included 
in the SDK. Each XML file has a corresponding DTD which it should validate against. 
The DTDs also serve to document the contents of these XML files.

catalog-dependencies.xml
  Dependencies of each calc function in Rusle2, with its inputs (terms) and outputs (affecteds).
catalog.rusle.xml
  The description of the main part of the Rusle2 scientific model.
  In particular, this lists all parameters accessible to an external user, and their units, default and legal values,...
catalog.shared.xml
  The description of the small common part of the model, which may be shared by other models.
data.dtd
  The DTD for the XML data files saved by Rusle2.
  These XML files can be loaded and saved as Win32 files for testing purposes.
skeletons.dtd
  The DTD for the management "skeleton" files exported by Rusle2 (*.SKEL).
  These can be imported by other models such as WEPS.
