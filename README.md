# Hazelcast C++ Client

C++ client implementation for [Hazelcast](https://github.com/hazelcast/hazelcast), the open source in-memory data grid. A comparison of features supported by the C++ Client vs the Java client can be found [here](http://docs.hazelcast.org/docs/3.5/manual/html/javaclient.html).

## Documentation

API Documentation can be generated via doxygen from root with following command

```
doxygen docsConfig
```

## How to build

### For mac release

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release
	cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release

### For mac code coverage

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug -DHZ_CODE_COVERAGE=ON
	gcovr -u -e .*external.* -e SimpleMapTest.h --html --html-details -o coverage.html 

### For mac Xcode development

  	cmake .. -G Xcode -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug

	cmake .. -G Xcode -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=archive -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=library
### For Linux Address Sanitizer

        cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug -DHZ_ADDRESS_SANITIZER=ON

### valgrind sample run with suppresions

        valgrind --leak-check=yes  --gen-suppressions=all --suppressions=../test.sup  ./hazelcast/test/clientTest_STATIC_64.exe  > log.t

### For linux release

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Release
	cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Release

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release
	cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release

### For linux valgrind

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug -DHZ_VALGRIND=ON

### For windows release

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Release
	cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Release
	cmake .. -G "Visual Studio 10 Win64"  -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release
	cmake .. -G "Visual Studio 10 Win64"  -DHZ_LIB_TYPE=SHARED -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release

	MSBuild.exe HazelcastClient.sln /property:Configuration=Release

### For windows debug

	cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Debug
	cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=32 -DCMAKE_BUILD_TYPE=Debug
	cmake .. -G "Visual Studio 10 Win64"  -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug
	cmake .. -G "Visual Studio 10 Win64"  -DHZ_LIB_TYPE=SHARED -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Debug

	MSBuild.exe HazelcastClient.sln /property:TreatWarningsAsErrors=true /property:Configuration=Debug

## Mail Group

Please join the mail group if you are interested in using or developing Hazelcast.

[http://groups.google.com/group/hazelcast](http://groups.google.com/group/hazelcast)

## License

Hazelcast C++ Client is available under the Apache 2 License. Please see the [Licensing appendix](http://docs.hazelcast.org/docs/latest/manual/html-single/hazelcast-documentation.html#license-questions) for more information.

## Copyright

Copyright (c) 2008-2015, Hazelcast, Inc. All Rights Reserved.

Visit [www.hazelcast.com](http://www.hazelcast.com/) for more info.
