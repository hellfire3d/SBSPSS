@if exist Speech.Scr del Speech.Scr

@echo ########### >>Speech.Scr
@echo # English # >>Speech.Scr
@echo ########### >>Speech.Scr
@for %file in (speech\eng\*.wav) do call _makescr.bat Eng E %@name["%file"] >>Speech.Scr

@echo ########### >>Speech.Scr
@echo # French  # >>Speech.Scr
@echo ########### >>Speech.Scr
@for %file in (speech\Fre\*.wav) do call _makescr.bat Fre E %@name["%file"] >>Speech.Scr

@echo ########### >>Speech.Scr
@echo # German  # >>Speech.Scr
@echo ########### >>Speech.Scr
@for %file in (speech\Ger\*.wav) do call _makescr.bat Ger E %@name["%file"] >>Speech.Scr

