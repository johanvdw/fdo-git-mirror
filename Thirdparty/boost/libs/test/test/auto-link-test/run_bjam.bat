bjam "-sTOOLS=vc-7_1" "-sSTLPORT_VERSION=4.6.2" "-sSTLPORT_ROOT=D:\Source Code\STLport-4.6.2" "-sSTLPORT_PATH=D:\Source Code" "-sVC71_ROOT=C:\Compilers\Microsoft Visual Studio .NET 2003\Vc7" -sBUILD="release debug <threading>multi/single <runtime-link>static/dynamic" test 2>&1 | tee res
