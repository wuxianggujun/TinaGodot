@echo off
echo Cleaning old project files...
del /F /Q godot.vcxproj 2>nul
del /F /Q godot.sln 2>nul
del /F /Q godot.vcxproj.filters 2>nul

echo Regenerating Visual Studio project...
scons platform=windows vsproj=yes

echo Done!
pause
