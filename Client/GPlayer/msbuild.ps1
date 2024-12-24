# $VS_INSTALL_DIR = "E:\VS2017\Common7\IDE\devenv.exe"
$SOLUTION_DIR = "E:\Desktop\languguetest\CplusplusProject\GPlayer"
$SOLUTION_NAME = "GPlayer.sln"
$CONFIGURATION = "Debug"


msbuild GPlayer.sln /p:Configuration=Debug;Platform=x64
msbuild GtooPlayer.vcxproj /p:Configuration=Debug;Platform=x64