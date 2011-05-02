#include "Threads.h"

Thread::Thread() {

}

Thread::~Thread() { 
	CloseHandle(thread_handle);
}

DWORD thread_ftn(LPVOID T) {
	Thread* t = static_cast<Thread*>(T);
	t->run();
	return NULL;
}

void Thread::start() {
	thread_handle = CreateThread(
		NULL, // default security
		0, // default stack size
		(LPTHREAD_START_ROUTINE)&thread_ftn, // thread function name
		(LPVOID)this, // argument to thread function
		CREATE_SUSPENDED, // use default creation flags
		&thread_id);
}

void Thread::join() {
	WaitForSingleObject(thread_handle, INFINITE);
}

void Thread::resume() {
	ResumeThread(thread_handle);
}