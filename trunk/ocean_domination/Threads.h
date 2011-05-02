#ifndef THREADS_H
#define THREADS_H

#include <windows.h>

class Thread {
public:
	Thread();
	virtual ~Thread();
	virtual void start();
	virtual void join();
	virtual void resume();
	virtual DWORD get_id() const { return thread_id; }

protected:
	virtual void run() = 0;
	friend DWORD thread_ftn(LPVOID T);
	HANDLE thread_handle;
	DWORD thread_id;

private:
	Thread(const Thread& src);
	Thread& operator=(const Thread& rhs);
};

class InitializationThread : public Thread {
public:
	InitializationThread(HGLRC renderContext, HDC currentHDC);

protected:
	virtual void run();

private:
	HGLRC context;
	HDC hdc;
};

#endif