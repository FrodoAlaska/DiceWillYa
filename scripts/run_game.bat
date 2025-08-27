pushd build 

cmake --build . --config Release --parallel 12
if %ERRORLEVEL% NEQ 0 goto error

.\Release\dice_will_ya.exe 

popd

:error 
echo "Error occured while trying to execute the game"
popd
