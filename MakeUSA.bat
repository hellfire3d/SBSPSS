del out\*.* /s /y /e >nul
@tools\make -r -f makefile.gfx VERSION=DEBUG TERRITORY=USA USER_NAME=CDBUILD
@tools\make -r -f makefile.gaz VERSION=DEBUG TERRITORY=USA USER_NAME=CDBUILD
ren cdbuild CdBuild.Debug
@tools\make -r -f makefile.gfx VERSION=FINAL TERRITORY=USA USER_NAME=CDBUILD
@tools\make -r -f makefile.gaz VERSION=FINAL TERRITORY=USA USER_NAME=CDBUILD
ren cdbuild CdBuild.Final
