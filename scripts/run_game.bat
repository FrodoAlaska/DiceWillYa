pushd build 

cmake --build . --config Debug --parallel 12
if %ERRORLEVEL% NEQ 0 goto error

.\Debug\epona.exe 

popd

:error 
echo "Error occured while trying to execute the game"
popd
