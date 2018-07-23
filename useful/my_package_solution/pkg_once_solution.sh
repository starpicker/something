#!/bin/sh
echo 
echo _________________________________________________________________
echo FUNCTION:
echo 
echo package each pkg folder and move the .pkgs from each pkg folder 
echo to the corresponding output folder in ONCE
echo 
echo _________________________________________________________________

bash get_ini_path
start /wait pkg.bat 

bash move_pkg_from_pkg_to_output


