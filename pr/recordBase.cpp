#include "recordBase.h"

ULARGE_INTEGER getFIleTime_difference(FILETIME& f1, FILETIME& f2)
{
	ULARGE_INTEGER u1 = { f1.dwLowDateTime, f1.dwHighDateTime };
	ULARGE_INTEGER u2 = { f2.dwLowDateTime, f2.dwHighDateTime };
	ULARGE_INTEGER out;
	out.QuadPart = u2.QuadPart - u1.QuadPart;
	return out;

}


