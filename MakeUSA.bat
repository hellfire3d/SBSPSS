del out\*.* /s /y /e >nul
@tools\make -r -f makefile.gfx VERSION=DEBUG TERRITORY=USA USER_NAME=CDBUILD
@tools\make -r -f makefile.gaz VERSION=DEBUG TERRITORY=USA USER_NAME=CDBUILD

