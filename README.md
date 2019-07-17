# DungeonTools
A set of tools for the game master to use running games in the 5th edition of the world's most popular fantasy role-playing game.

Note that this application uses the Sqlite embedded database engine.  By default, it will create a subdirectory in your home directory called ".dungeontools".  In here it will store two files, "dungeontools.ini" where your local settings will be stored (e.g., main window location).  After you run the application for the first time, you will also find "dungeontools-db.sqlite".  This is the Sqlite database file that the application uses to store all its information.  In general, you should only edit either if you know what you're doing.  The INI file is a standard text file, so any text editor can be used.  The dunbeontools-db.sqlite will require a Sqlite database enabled application to edit.

To Build:
This software was written using Qt Creator v5.13.  To build it, you will need that tool (as I've not yet setup a command line build process).