//
//  common.cpp
//  NGHcaller
//
//  Copyright (c) 2013 Bruno Nevado, GNU license.


#include "common.h"

long getMemoryUsage(void) {
  struct rusage r_usage;
  getrusage(RUSAGE_SELF,&r_usage);
  return(r_usage.ru_maxrss);
}

long getMemoryLimit(void) {
	/*
	The getrlimit() function shall get, and the setrlimit() function shall set, limits on the consumption of a variety of resources.
	Each call to either getrlimit() or setrlimit() identifies a specific resource to be operated upon as well as a resource limit. A resource limit is represented by an rlimit structure. The rlim_cur member specifies the current or soft limit and the rlim_max member specifies the maximum or hard limit. Soft limits may be changed by a process to any value that is less than or equal to the hard limit. A process may (irreversibly) lower its hard limit to any value that is greater than or equal to the soft limit. Only a process with appropriate privileges can raise a hard limit. Both hard and soft limits can be changed in a single call to setrlimit() subject to the constraints described above.
	The value RLIM_INFINITY, defined in <sys/resource.h>, shall be considered to be larger than any other limit value. If a call to getrlimit() returns RLIM_INFINITY for a resource, it means the implementation shall not enforce limits on that resource. Specifying RLIM_INFINITY as any resource limit value on a successful call to setrlimit() shall inhibit enforcement of that resource limit.
	The following resources are defined:

	RLIMIT_CORE
	This is the maximum size of a core file, in bytes, that may be created by a process. A limit of 0 shall prevent the creation of a core file. If this limit is exceeded, the writing of a core file shall terminate at this size.
	RLIMIT_CPU
	This is the maximum amount of CPU time, in seconds, used by a process. If this limit is exceeded, SIGXCPU shall be generated for the process. If the process is catching or ignoring SIGXCPU, or all threads belonging to that process are blocking SIGXCPU, the behavior is unspecified.
	RLIMIT_DATA
	This is the maximum size of a process' data segment, in bytes. If this limit is exceeded, the malloc() function shall fail with errno set to [ENOMEM].
	RLIMIT_FSIZE
	This is the maximum size of a file, in bytes, that may be created by a process. If a write or truncate operation would cause this limit to be exceeded, SIGXFSZ shall be generated for the thread. If the thread is blocking, or the process is catching or ignoring SIGXFSZ, continued attempts to increase the size of a file from end-of-file to beyond the limit shall fail with errno set to [EFBIG].
	RLIMIT_NOFILE
	This is a number one greater than the maximum value that the system may assign to a newly-created descriptor. If this limit is exceeded, functions that allocate a file descriptor shall fail with errno set to [EMFILE]. This limit constrains the number of file descriptors that a process may allocate.
	RLIMIT_STACK
	This is the maximum size of the initial thread's stack, in bytes. The implementation does not automatically grow the stack beyond this limit. If this limit is exceeded, SIGSEGV shall be generated for the thread. If the thread is blocking SIGSEGV, or the process is ignoring or catching SIGSEGV and has not made arrangements to use an alternate stack, the disposition of SIGSEGV shall be set to SIG_DFL before it is generated.
	RLIMIT_AS
	This is the maximum size of a process' total available memory, in bytes. If this limit is exceeded, the malloc() and mmap() functions shall fail with errno set to [ENOMEM]. In addition, the automatic stack growth fails with the effects outlined above.

	When using the getrlimit() function, if a resource limit can be represented correctly in an object of type rlim_t, then its representation is returned; otherwise, if the value of the resource limit is equal to that of the corresponding saved hard limit, the value returned shall be RLIM_SAVED_MAX; otherwise, the value returned shall be RLIM_SAVED_CUR.
	When using the setrlimit() function, if the requested new limit is RLIM_INFINITY, the new limit shall be "no limit''; otherwise, if the requested new limit is RLIM_SAVED_MAX, the new limit shall be the corresponding saved hard limit; otherwise, if the requested new limit is RLIM_SAVED_CUR, the new limit shall be the corresponding saved soft limit; otherwise, the new limit shall be the requested value. In addition, if the corresponding saved limit can be represented correctly in an object of type rlim_t then it shall be overwritten with the new limit.
	The result of setting a limit to RLIM_SAVED_MAX or RLIM_SAVED_CUR is unspecified unless a previous call to getrlimit() returned that value as the soft or hard limit for the corresponding resource limit.
	The determination of whether a limit can be correctly represented in an object of type rlim_t is implementation-defined. For example, some implementations permit a limit whose value is greater than RLIM_INFINITY and others do not.
	The exec family of functions shall cause resource limits to be saved.
	*/
  struct rlimit r_limit;
  getrlimit(RLIMIT_RSS,&r_limit);
  /*
  The <sys/resource.h> header shall define the rlimit structure that includes at least the following members:
  rlim_t rlim_cur  The current (soft) limit.
  rlim_t rlim_max  The hard limit.
  */
  return(r_limit.rlim_max);
}


/*
//void tokenize(const char* str, char delim, std::vector<std::string>& tokenList)
void msplit2(const std::string* str, char delim, std::vector<std::string>& tokenList)
{
    tokenList.clear();
    int start = -1;
    for (int i = 0; str[i]; ++i)
    {
        if (str[i] == delim)
        {
            tokenList.push_back(std::string(str + start + 1, str + i));
            start = i;
        }
    }
}
 static void split(const string& str, char d, std::list& list){
 const char* s(str.c_str());
 while(* s ){
 const char* item(s);
 while( *s && *(s++) != d ); // only mv s fwd if we are not at string end
 list.push_back(string(item)); // template over list if needed
 }
 }
 
 static void split(const std::string& str, char d, std::list& list){
 const char* s(str.c_str());
 while(*s){
 const char* item(s);
 while(*s && *s != d) s++; // only mv s fwd if we are not at string end
 list.push_back(std::string(item, s-item)); // template over list if needed
 if (*s && !*(++s)) list.push_back("");
 }
 }
 
 void dump(std::list& words) {
 std::list::iterator n = words.begin();
 while (n != words.end()) {
 std::cout << '"' << *n++ << '"' << std::endl;
 }
 std::cout << std::endl;
 }
 

 
*/


#ifdef MSPLITNEW
void msplit2( const std::string& s , const char& delim,  std::vector<std::string> * output){
    const char* str(s.c_str());
    while(*str){
        const char* item(str);
        while(*str && *str != delim) str++; // only mv s fwd if we are not at string end
        output->push_back(std::string(item, str-item)); // template over list if needed
        if (*str && !*(++str)) output->push_back("");
    }
}
#else
void msplit2( const std::string& s , const char& delim,  std::vector<std::string> * output){
    unsigned long start = 0U;
    unsigned long end = s.find(delim);
    while (end != std::string::npos)
    {
        output->push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delim, start);
    }
    output->push_back(s.substr(start, end - start));
}
#endif



double diffclock( clock_t clock1, clock_t clock2 ) {

      double diffticks = clock1 - clock2;
      double diffms    = diffticks / ( CLOCKS_PER_SEC / 1000 );

      return diffms;
}


double Mtime( clock_t& time ) {

    clock_t c = clock();
    double diffticks = c - time;
    double diffms    = diffticks / ( CLOCKS_PER_SEC / 1000 );
    time = c;

    return diffms;
}



