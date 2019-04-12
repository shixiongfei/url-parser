solution ( "url-parser-test" )
  configurations { "Release", "Debug" }
  platforms { "x64" }

  if _ACTION == "clean" then
    os.rmdir(".vs")
    os.rmdir("test")
    os.remove("url-parser-test.VC.db")
    os.remove("url-parser-test.sln")
    os.remove("url-parser-test.vcxproj")
    os.remove("url-parser-test.vcxproj.filters")
    os.remove("url-parser-test.vcxproj.user")
    os.remove("url-parser-test.make")
    os.remove("Makefile")
    return
  end

  -- A project defines one build target
  project ( "url-parser-test" )
  kind ( "ConsoleApp" )
  language ( "C" )
  targetname ("url-parser-test")
  files { "./*.h", "./*.c" }
  defines { "_UNICODE" }
  staticruntime "On"

  configuration ( "Release" )
    optimize "On"
    objdir ( "./test/tmp" )
    targetdir ( "./test" )
    defines { "NDEBUG", "_NDEBUG" }

  configuration ( "Debug" )
    symbols "On"
    objdir ( "./test/tmp" )
    targetdir ( "./test" )
    defines { "DEBUG", "_DEBUG" }

  configuration ( "vs*" )
    defines { "WIN32", "_WIN32", "_WINDOWS",
              "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE",
              "_CRT_NONSTDC_NO_DEPRECATE", "_WINSOCK_DEPRECATED_NO_WARNINGS" }

  configuration ( "gmake" )
    warnings  "Default" --"Extra"
    defines { "LINUX_OR_MACOSX" }
    links { "iconv" }

  configuration { "gmake", "macosx" }
    defines { "__APPLE__", "__MACH__", "__MRC__", "macintosh" }

  configuration { "gmake", "linux" }
    defines { "__linux__" }
