========================================================================
    CONSOLE APPLICATION : PipeClient Project Overview
========================================================================

StreamBase task

Requirements
- The client should be able to connect to the server through a NamedPipe
  It uses "\\.\pipe\mynamedpipe" named pipe and exchanges dapa packed by protobuf
- The client should be able to make both sync and async calls to the server
  Taken from official MS example https://docs.microsoft.com/ru-ru/windows/desktop/ipc/named-pipe-client
- The client should be able to send trivial data (strings, numbers) to the server
  Data is sent as data buffer and can be serialized using protobuf
- The client should be able to create objects on the server (based on req-7 below), retrieve them, their attributes and call methods on them


Pre requirements:
1. Install Protobuf using vcpkg
vcpkg install protobuf protobuf:x64-windows
Follow official guide:
https://github.com/protocolbuffers/protobuf/blob/master/src/README.md
2. Add protoc.exe to PATH
3. Generate Object.proto into pb.h and pb.cc files

TODO:
make custom build step for Object.proto to generate pb.h and pb.cc files

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named PipeClient.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
