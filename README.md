# BG3_ModdingTool
BG3 Modding Tool is a QT Creator C++ based GUI app that allows for easier and more convenient modifying of Baldur's Gate 3 files in order to make your own mods. 

Changelog:
v.0.1:
- initial setup with basic data importing from the correct files
- changes to the textboxes are saved as changes to the corresponding classes in the program
- all changes can be saved (exported) to the corresponding files

v.0.2:
- the app correctly read and outputs files: Progressions.lsx and ClassDescriptions.lsx
- after mouse click on the boosts textbox in progressions a table is shown with each boost as a single row in the table (normally they are split by ';', written in one line)
- editing rows in the table changes the actual classes and progressions, etc.

v.0.3:
- working context menu for choosing boosts from possible options

v.0.4:
- changed the boostsTable so that it contains lineEdits instead of tableItems (allows e.g. to track where the cursor is in the lineEdit, etc.)
- started working on the autofill system (hints to auto complete what the user starts writing when trying to add/modify boosts) 

v.0.5:
- added this very changelog file :)
- added working autofill system (hints to auto complete what the user starts writing) 
	- so far only for adding boosts into progressions
	- not all lists are correct
	- does not yet support taking data from multiple lists at once to display as hints (e.g. Proficiency can take as an argument either a skill or weapon or armor proficiency)

v.0.6:
- added multiple lists support to the autofill system
- changed the tooltips in the context menu (now only shows examples of usage and each action has its own tooltip)
- added history file with what files were previously used
- made it possible to import data from multiple ClassDescriptions.lsx and Progressions.lsx files at once
- added default game files to import data from 
- added support for displaying and modifying multiclass progressions
- made changing progression level work correctly



TO DO:
- add autofill support for other Progressions Attributes apart from boosts (e.g. selectors)
- differentiate between classes and subclasses (especially in the GUI as mechanics is almost the same)
- change the GUI 