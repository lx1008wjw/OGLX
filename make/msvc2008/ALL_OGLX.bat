@If DEFINED ProgramFiles(x86) Set programs=%ProgramFiles(x86)%
@If Not DEFINED ProgramFiles(x86) Set programs=%ProgramFiles%

path %programs%\Microsoft Visual Studio 9.0\Common7\IDE;%PATH%

devenv OGLX.sln /rebuild "Release|Win32"
devenv OGLX.sln /rebuild "Debug|Win32"
devenv OGLX.sln /rebuild "Release DLL|Win32"
devenv OGLX.sln /rebuild "Release|x64"
devenv OGLX.sln /rebuild "ES1 Release|Win32"
devenv OGLX.sln /rebuild "ES2 Release|Win32"
devenv OGLX.sln /rebuild "ES2 Debug|Win32"
devenv OGLX.sln /rebuild "SC Release|Win32"
devenv OGLX.sln /rebuild "SC2 Release|Win32"
devenv OGLX.sln /rebuild "GL13|Win32"
devenv OGLX.sln /rebuild "GL13|x64"
