echo "Cleanup release directories"
rm -rf ./Release*
rm -rf ./cpp

echo "Compiling Static 32bit library"
mkdir ReleaseStatic32
cd .\ReleaseStatic32
cd
cmake .. -DHZ_LIB_TYPE=STATIC -DHZ_BIT=32  -DCMAKE_BUILD_TYPE=Release 
MSBuild.exe HazelcastClient.sln /m /property:Configuration=Release /p:VisualStudioVersion=12.0
cd ..

echo "Compiling Shared 32bit library"
mkdir ReleaseShared32
cd .\ReleaseShared32
cd;
cmake .. -DHZ_LIB_TYPE=SHARED -DHZ_BIT=32  -DCMAKE_BUILD_TYPE=Release 
MSBuild.exe HazelcastClient.sln /m /property:Configuration=Release /p:VisualStudioVersion=12.0
cd ..

echo "Moving 32bit libraries to target"
mkdir .\cpp\Windows_32\hazelcast\lib\static
mkdir .\cpp\Windows_32\hazelcast\lib\shared
mkdir .\cpp\Windows_32\hazelcast\include\hazelcast\
mkdir .\cpp\Windows_32\external\include
mkdir .\cpp\Windows_32\examples

xcopy /S /Q hazelcast\include\hazelcast\* cpp\Windows_32\hazelcast\include\hazelcast\

copy ReleaseShared32\Release\HazelcastClient*  cpp\Windows_32\hazelcast\lib\shared\
copy ReleaseStatic32\Release\HazelcastClient*  cpp\Windows_32\hazelcast\lib\static\

echo "Moving 32bit external libraries to target"
xcopy /S /Q external\include\* cpp\Windows_32\external\include\

echo "Moving 32bit examples to target"
copy examples\*cpp  cpp\Windows_32\examples
copy ReleaseStatic32\examples\Release\*exe  cpp\Windows_32\examples

echo "Clearing temporary 32bit librares"
rm -rf .\ReleaseShared32
rm -rf .\ReleaseStatic32

echo "Compiling Static 64bit library"
mkdir ReleaseStatic64
cd .\ReleaseStatic64
cd;
cmake .. -G "Visual Studio 12 Win64" -DHZ_LIB_TYPE=STATIC -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release 
MSBuild.exe HazelcastClient.sln /m /property:Configuration=Release /p:VisualStudioVersion=12.0
cd ..

echo "Compiling Shared 64bit library"
mkdir ReleaseShared64
cd .\ReleaseShared64
cd;
cmake .. -G "Visual Studio 12 Win64" -DHZ_LIB_TYPE=SHARED -DHZ_BIT=64 -DCMAKE_BUILD_TYPE=Release 
MSBuild.exe HazelcastClient.sln /m /property:Configuration=Release /p:VisualStudioVersion=12.0
cd ..

echo "Moving 64bit libraries to target"
mkdir .\cpp\Windows_64\hazelcast\lib\static
mkdir .\cpp\Windows_64\hazelcast\lib\shared
mkdir .\cpp\Windows_64\hazelcast\include\hazelcast\
mkdir .\cpp\Windows_64\external\include
mkdir .\cpp\Windows_64\examples

xcopy /S /Q hazelcast\include\hazelcast\* cpp\Windows_64\hazelcast\include\hazelcast\

copy ReleaseShared64\Release\HazelcastClient*  cpp\Windows_64\hazelcast\lib\shared\
copy ReleaseStatic64\Release\HazelcastClient*  cpp\Windows_64\hazelcast\lib\static\

echo "Moving 64bit external libraries to target"
xcopy /S /Q external\include\* cpp\Windows_64\external\include\

echo "Moving 64bit examples to target"
copy examples\*cpp  cpp\Windows_64\examples
copy ReleaseStatic64\examples\Release\*exe  cpp\Windows_64\examples

echo "Clearing tempraroy 64bit librares"
rm -rf ./ReleaseShared64
rm -rf ./ReleaseStatic64

