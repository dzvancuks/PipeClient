// PipeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Object.pb.h"
#include "ClassA.h"

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

class Pipe // singleton
{
public:
	static const int BUFSIZE = 512;
	static HANDLE handle()
	{
		static Pipe instance;
		return instance.hPipe;
	}
private:
	HANDLE hPipe = nullptr;
	const LPTSTR pipe_name = TEXT("\\\\.\\pipe\\mynamedpipe");

	Pipe()
	{
		while (1)
		{
			hPipe = CreateFile(
				pipe_name,   // pipe name 
				GENERIC_READ |  // read and write access 
				GENERIC_WRITE,
				0,              // no sharing 
				NULL,           // default security attributes
				OPEN_EXISTING,  // opens existing pipe 
				0,              // default attributes 
				NULL);          // no template file 

								// Break if the pipe handle is valid. 

			if (hPipe != INVALID_HANDLE_VALUE)
				break;

			// Exit if an error other than ERROR_PIPE_BUSY occurs. 
			assert(GetLastError() != ERROR_PIPE_BUSY);

			// All pipe instances are busy, so wait for 20 seconds. 
			assert(!WaitNamedPipe(pipe_name, 20000));
		}

		// The pipe connected; change to message-read mode. 
		DWORD dwMode = PIPE_READMODE_MESSAGE;
		assert(SetNamedPipeHandleState(
			hPipe,    // pipe handle 
			&dwMode,  // new pipe mode 
			NULL,     // don't set maximum bytes 
			NULL)     // don't set maximum time 
		);
	}

	~Pipe()
	{
		CloseHandle(hPipe);
	}

};

bool send_receive(const Object& to_send, Object& to_receive)
{
	char  buf[Pipe::BUFSIZE];
	BOOL   success = FALSE;
	DWORD  read, written;

	to_send.SerializeToArray(buf, Pipe::BUFSIZE);

	success = WriteFile(
		Pipe::handle(),
		buf,
		to_send.ByteSizeLong(), // message length 
		&written,               // bytes written 
		NULL);                  // not overlapped 

	if (!success)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return false;
	}

	printf("\nSent object message, receiving reply as follows:\n");

	do
	{
		// Read from the pipe. 

		success = ReadFile(
			Pipe::handle(),
			buf,    // buffer to receive reply 
			Pipe::BUFSIZE,  // size of buffer 
			&read,  // number of bytes read 
			NULL);    // not overlapped 

		if (!success && GetLastError() != ERROR_MORE_DATA)
			break;

		to_receive.ParseFromArray(buf, Pipe::BUFSIZE);
		printf("Received new object %s\n", to_receive.name().c_str());

	} while (!success);  // repeat loop if ERROR_MORE_DATA 

	if (!success)
	{
		_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
		return false;
	}

	return true;
}

bool run_tests()
{
	Object to_retrieve_class_a, retrieved_class_a;
	to_retrieve_class_a.set_action(Action::RETRIEVE);
	to_retrieve_class_a.set_type(Type::NEW_CLASS_A);

	if (!send_receive(to_retrieve_class_a, retrieved_class_a))
	{
		return false;
	}

	ClassA* class_a_ptr = static_cast<ClassA*>(malloc(sizeof ClassA));
	memcpy(class_a_ptr, retrieved_class_a.data().data(), sizeof ClassA);
	printf("String data member is \"%s\"\n", class_a_ptr->get_string_member().c_str());
}

int _tmain(int argc, TCHAR *argv[])
{
	run_tests();
	
	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();

	return 0;
}
